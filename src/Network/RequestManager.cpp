#include "RequestManager.h"

#include <QNetworkAccessManager>
#include <QNetworkCookie>
#include <QNetworkCookieJar>


Q_LOGGING_CATEGORY(RequestManagerLog, "network.requestManager");

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
    emit urlsEnqueued(urls.size());

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
    emit urlsEnqueued();

    urlsQueue.enqueue(url);

    if (!requestTimer.isActive())
    {
        requestTimer.start(requestIntervalMs);
    }
}

void RequestManager::onInterrupt()
{
    qCDebug(RequestManagerLog) << "Interrupted";

    isInterrupted = true;

    cancelAll();
}

void RequestManager::onFinished(QNetworkReply* reply)
{
    if (isInterrupted)
    {
        qCDebug(RequestManagerLog) << "Interrupted";

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

        qCWarning(RequestManagerLog) << "statusCode:" << statusCode;

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
                qCWarning(RequestManagerLog) << "something to do with other codes..."; // TODO
            }
        }

        emit meaninglessRepliesReceived();
        return;
    }


    const QList<QNetworkCookie> cookies = networkManager.cookieJar()->cookiesForUrl(url);

    if (!cookiesInitialized)
    {
        if (!cookies.isEmpty())
        {
            cookiesInitialized = true;
            qCInfo(RequestManagerLog) << "Cookies initialized:" << cookies.size();
        }
        else
        {
            qCInfo(RequestManagerLog) << "No cookies received.";
        }
    }

    emit meaningfulRepliesReceived();
    emit receivedReply(reply->readAll(), url);

    reply->deleteLater();
}

void RequestManager::startNext()
{
    if (urlsQueue.isEmpty() && activeReplies.isEmpty())
    {
        qCDebug(RequestManagerLog) << "Requesting is done!";

        stopTimers();
        return;
    }

    if (isInterrupted)
    {
        qCDebug(RequestManagerLog) << "Interrupted";
        return;
    }

    if (requestCooldownTimer.isActive())
        return;

    const unsigned _maxConcurrent = cookiesInitialized ? maxConcurrent : 1;

    if (!urlsQueue.isEmpty() && activeReplies.size() < _maxConcurrent)
    {
        emit requestsStarted();
        emit urlsDequeued();

        const QUrl url = urlsQueue.dequeue();
        qCDebug(RequestManagerLog) << url;

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
    emit requestsFinished(activeReplies.size());
    for (QNetworkReply* reply : activeReplies)
    {
        reply->abort();
    }

    activeReplies.clear();
}

void RequestManager::clearQueue()
{
    emit urlsDequeued(urlsQueue.size());
    urlsQueue.clear();
}