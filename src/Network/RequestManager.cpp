#include "RequestManager.h"

#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include <QNetworkCookie>


Q_LOGGING_CATEGORY(NetworkLog, "network");

RequestManager::RequestManager(QObject* parent) :
    QObject(parent),
    networkManager(*new QNetworkAccessManager(this)),
    urlsQueue(),
    activeReplies(),
    cookiesInitialized(false),
    isInterrupted(false),
    requestTimer(this),
    requestsPerSecond(8),
    requestIntervalMs(1000 / requestsPerSecond),
    requestCooldownTimer(this),
    requestCooldown(5000),
    maxConcurrent(8)
{
    QObject::connect(&networkManager, &QNetworkAccessManager::finished, this, &RequestManager::onFinished);

    requestCooldownTimer.setSingleShot(true);

    connect(&requestTimer, &QTimer::timeout, this, &RequestManager::startNext);
}

void RequestManager::setRequestsPerSecond(unsigned int _requestsPerSecond)
{
    requestsPerSecond = std::max(1u, _requestsPerSecond);
    requestIntervalMs = 1000 / requestsPerSecond;

    if (requestTimer.isActive())
    {
        requestTimer.start(requestIntervalMs);
    }
}

void RequestManager::setMaxConcurrent(const unsigned int _maxConcurrent)
{
    maxConcurrent = std::max(1u, _maxConcurrent);
}

void RequestManager::setRequestCooldownMs(const unsigned int cooldown)
{
    requestCooldown = cooldown;
}

quint64 RequestManager::getErrorCount() const
{
    return errorCount;
}

void RequestManager::reset()
{
    isInterrupted = false;

    cancelAll();

    networkManager.setCookieJar(new QNetworkCookieJar(&networkManager));
    cookiesInitialized = false;
}

void RequestManager::addUrls(const QList<QUrl>& urls)
{
    for (const QUrl& url : urls)
    {
        urlsQueue.enqueue(url);
    }

    if (!requestTimer.isActive())
    {
        requestTimer.start(requestIntervalMs);
    }
}

void RequestManager::addUrl(const QUrl& url)
{
    urlsQueue.enqueue(url);

    if (!requestTimer.isActive())
    {
        requestTimer.start(requestIntervalMs);
    }
}

void RequestManager::onInterrupt()
{
    qCDebug(NetworkLog) << "Interrupted";

    isInterrupted = true;

    cancelAll();
}

void RequestManager::onFinished(QNetworkReply* reply)
{
    if (isInterrupted)
    {
        qCDebug(NetworkLog) << "Interrupted";

        reply->deleteLater();
        return;
    }

    activeReplies.remove(reply);

    const QNetworkReply::NetworkError errorState = reply->error();
    const QUrl url = reply->url();

    emit replyError(errorState);
    if (errorState != QNetworkReply::NoError)
    {
        const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        qCWarning(NetworkLog) << "statusCode:" << statusCode;

        reply->deleteLater();

        switch (statusCode)
        {
            case 429:
            {
                errorCount++;
                requestCooldownTimer.start(requestCooldown);
                addUrl(url);
                break;
            }
            default:
            {
                qCWarning(NetworkLog) << "something to do with other codes..."; // TODO
            }
        }

        return;
    }


    const QList<QNetworkCookie> cookies = networkManager.cookieJar()->cookiesForUrl(url);

    if (!cookiesInitialized)
    {
        if (!cookies.isEmpty())
        {
            cookiesInitialized = true;
            qCInfo(NetworkLog) << "Cookies initialized:" << cookies.size();
        }
        else
        {
            qCInfo(NetworkLog) << "No cookies received.";
        }
    }

    emit receivedReply(reply->readAll(), url);

    reply->deleteLater();
}

void RequestManager::startNext()
{
    if (urlsQueue.isEmpty() && activeReplies.isEmpty())
    {
        qCDebug(NetworkLog) << "Requesting is done!";

        stopTimers();

        emit requestsFinished();
        return;
    }

    if (isInterrupted)
    {
        qCDebug(NetworkLog) << "Interrupted";
        return;
    }

    if (requestCooldownTimer.isActive())
        return;

    const unsigned _maxConcurrent = cookiesInitialized ? maxConcurrent : 1;

    if (!urlsQueue.isEmpty() && activeReplies.size() < _maxConcurrent)
    {
        const QUrl url = urlsQueue.dequeue();
        qCDebug(NetworkLog) << url;

        QNetworkRequest request(url);
        request.setRawHeader("Accept-Language", "en-US,en;q=0.9");

        QNetworkReply* reply = networkManager.get(request);

        activeReplies.insert(reply);
    }
}

void RequestManager::cancelAll()
{
    stopTimers();
    cancelReplies();
    clearQueue();
}

void RequestManager::stopTimers()
{
    requestTimer.stop();
    requestCooldownTimer.stop();
}

void RequestManager::cancelReplies()
{
    for (QNetworkReply* reply : activeReplies)
        reply->abort();

    activeReplies.clear();
}

void RequestManager::clearQueue()
{
    urlsQueue.clear();
}