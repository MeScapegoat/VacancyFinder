#include "LinkedInRequestStrategy.h"

#include <QDebug>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>

#include <algorithm>

Q_LOGGING_CATEGORY(LinkedInRequestStrategyLog, "network.linkedInRequestStrategy");


LinkedInRequestStrategy::LinkedInRequestStrategy(AbstractNetworkManager* const networkManager, QObject* parent) : 
    LinkedInRequestStrategy(networkManager, 8, 5000, 8, parent)
{
}

LinkedInRequestStrategy::LinkedInRequestStrategy(AbstractNetworkManager* const networkManager, 
                                                 size_t requestPerSecond, 
                                                 size_t requestCooldown, 
                                                 size_t maxConcurrent, 
                                                 QObject* parent) :
    AbstractRequestStrategy(networkManager, parent),
    m_activeReplies(),
    m_briefUrlsQueue(),
    m_detailsUrlsQueue(),
    m_requestTimer(this),
    m_requestCooldownTimer(this),
    m_requestsPerSecond(requestPerSecond),
    m_requestIntervalMs(1000 / m_requestsPerSecond),
    m_requestCooldown(requestCooldown),
    m_maxConcurrent(maxConcurrent),
    m_cookiesInitialized(false),
    m_minStart(),
    m_vacancyCountDetermined(false)
{
    m_requestCooldownTimer.setSingleShot(true);

    connect(&m_requestTimer, &QTimer::timeout, this, &LinkedInRequestStrategy::startNext);
}

void LinkedInRequestStrategy::reset()
{
    AbstractRequestStrategy::reset();

    cancelAll();

    m_networkManager->setCookieJar(new QNetworkCookieJar(m_networkManager));
    m_cookiesInitialized = false;

    m_minStart = std::nullopt;
    m_vacancyCountDetermined = false;
}

void LinkedInRequestStrategy::startNext()
{
    if (m_isInterrupted)
    {
        return;
    }

    if (!hasPendingRequests() && m_activeReplies.isEmpty())
    {
        stopTimers();
        return;
    }

    if (m_requestCooldownTimer.isActive())
        return;

    if (!m_vacancyCountDetermined && m_briefUrlsQueue.isEmpty())
    {
        m_vacancyCountDetermined = true;
        emit vacancyCountDetermined(m_detailsUrlsQueue.size());
    }

    const unsigned _maxConcurrent = m_cookiesInitialized ? m_maxConcurrent : 1; // m_networkManager->hasCookies()

    if (hasPendingRequests() && m_activeReplies.size() < _maxConcurrent)
    {
        emit requestsStarted();
        emit urlsDequeued();

        QUrl url;
        if (!m_briefUrlsQueue.isEmpty())
        {
            url = m_briefUrlsQueue.dequeue();
        }
        else
        {
            url = m_detailsUrlsQueue.dequeue();
        }

        
        qCDebug(LinkedInRequestStrategyLog) << "Requested url:" << url;

        QNetworkRequest request(url);
        request.setRawHeader("Accept-Language", "en-US,en;q=0.9");
        QNetworkReply* reply = m_networkManager->get(request);

        m_activeReplies.insert(reply);
    }
}

void LinkedInRequestStrategy::addUrls(const QList<QUrl>& urls)
{
    size_t addedUrls = 0;
    for (const QUrl& url : urls)
    {
        addedUrls += enqueueUrl(url);
    }

    if (addedUrls)
    {
        emit urlsEnqueued(addedUrls);
        startRequestTimer();
    }
}

void LinkedInRequestStrategy::addUrl(const QUrl& url)
{
    const bool isEnqueued = enqueueUrl(url);

    if (isEnqueued)
    {
        emit urlsEnqueued();
        startRequestTimer();
    }
}

void LinkedInRequestStrategy::interrupt()
{
    m_isInterrupted = true;

    cancelAll();
}

void LinkedInRequestStrategy::handleReceivedReply(QNetworkReply * reply)
{
    if (m_isInterrupted)
    {
        reply->deleteLater();
        return;
    }

    m_activeReplies.remove(reply);

    const QNetworkReply::NetworkError errorState = reply->error();
    const QUrl url = reply->url();
    const QString html = reply->readAll();
    const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    qCDebug(LinkedInRequestStrategyLog) << "Received url:" << url;

    reply->deleteLater();

    if (errorState != QNetworkReply::NoError)
    {

        switch (statusCode)
        {
            case 429:
            {
                m_requestCooldownTimer.start(m_requestCooldown);
                addUrl(url);
                break;
            }
            default:
            {
                qCWarning(LinkedInRequestStrategyLog) << "something to do with other codes..."; // TODO
                break;
            }
        }

        emit meaninglessRepliesReceived();
        return;
    }


    initCookies(url); // m_networkManager->initCookies(url);

    if (updateBriefUrlCutoff(html, url))
    {
        emit meaninglessRepliesReceived();
        return;
    }

    emit meaningfulRepliesReceived();
    emit receivedReply(html, url);
}

bool LinkedInRequestStrategy::enqueueUrl(const QUrl& url)
{
    const UrlType urlType = clasifyUrl(url);
    switch (urlType)
    {
        case UrlType::Brief:
        {
            m_briefUrlsQueue.enqueue(url);
            return true;
        }
        case UrlType::Details:
        {
            m_detailsUrlsQueue.enqueue(url);
            return true;
        }
        default:
        {
            qCWarning(LinkedInRequestStrategyLog) << "Unknown url type:" << url;
            return false;
        }
    }
}

void LinkedInRequestStrategy::clearQueues()
{
    if (!m_briefUrlsQueue.isEmpty())
    {
        emit urlsDequeued(m_briefUrlsQueue.size());
        m_briefUrlsQueue.clear();
    }

    if (!m_detailsUrlsQueue.isEmpty())
    {
        emit urlsDequeued(m_detailsUrlsQueue.size());
        m_detailsUrlsQueue.clear();
    }
}

bool LinkedInRequestStrategy::hasPendingRequests() const
{
    return !m_briefUrlsQueue.isEmpty() || !m_detailsUrlsQueue.isEmpty();
}

void LinkedInRequestStrategy::cancelAll()
{
    stopTimers();
    cancelReplies();
    clearQueues();
}

void LinkedInRequestStrategy::cancelReplies()
{
    if (m_activeReplies.isEmpty())
    {
        return;
    }

    emit requestsFinished(m_activeReplies.size());
    for (QNetworkReply* reply : m_activeReplies)
    {
        reply->abort();
    }

    m_activeReplies.clear();
}

void LinkedInRequestStrategy::stopTimers()
{
    m_requestTimer.stop();
    m_requestCooldownTimer.stop();
}

void LinkedInRequestStrategy::startRequestTimer()
{
    if (!m_requestTimer.isActive() && !m_isInterrupted)
    {
        m_requestTimer.start(m_requestIntervalMs);
    }
}

void LinkedInRequestStrategy::initCookies(const QUrl& url)
{
    if (!m_cookiesInitialized)
    {
        const QList<QNetworkCookie> cookies = m_networkManager->cookieJar()->cookiesForUrl(url);
        if (!cookies.isEmpty())
        {
            m_cookiesInitialized = true;
            qCDebug(LinkedInRequestStrategyLog) << "Cookies initialized:" << cookies.size();
        }
        else
        {
            qCDebug(LinkedInRequestStrategyLog) << "No cookies received.";
        }
    }
}

bool LinkedInRequestStrategy::updateBriefUrlCutoff(const QString& html, const QUrl& url)
{
    if (clasifyUrl(url) != UrlType::Brief)
    {
        return false;
    }

    if (html.contains("<li>"))
    {
        return false;
    }

    const int start = getStart(url);
    if (m_minStart.has_value() && m_minStart < start)
    {
        return false;
    }

    m_minStart = start;

    auto isAfterStartPred = [start](const QUrl& url)
    {
        return isAfterStart(start, url);
    };

    const qsizetype removed = m_briefUrlsQueue.removeIf(isAfterStartPred);
    emit urlsDequeued(removed);

    qCDebug(LinkedInRequestStrategyLog) << "Removed" << removed << "empty url from queue, starting at" << start;

    return true;
}

UrlType clasifyUrl(const QUrl& url)
{
    if (url.hasQuery())
        return UrlType::Brief;

    return UrlType::Details;
}

int getStart(const QUrl& url)
{
    const QUrlQuery query(url.query());
    return query.queryItemValue("start").toInt();
}

bool isAfterStart(int start, const QUrl& url)
{
    return getStart(url) > start;
}
