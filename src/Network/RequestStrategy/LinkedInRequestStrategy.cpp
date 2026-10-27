#include "LinkedInRequestStrategy.h"

#include <QDebug>
#include <QLoggingCategory>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QString>
#include <QUrlQuery>

#include <algorithm>

namespace
{
    constexpr qsizetype RequestsPerSecond = 8;
    constexpr qsizetype RequestCooldownMs = 5000;
    constexpr qsizetype MaxConcurrentRequests = 8;
    constexpr qsizetype MillisecondsPerSecond = 1000;
    constexpr qsizetype InitialConcurrencyLimit = 1;
    constexpr qsizetype TooManyRequestsStatusCode = 429;
}

Q_LOGGING_CATEGORY(LinkedInRequestStrategyLog, "network.linkedInRequestStrategy");


LinkedInRequestStrategy::LinkedInRequestStrategy(AbstractNetworkManager* const networkManager, QObject* parent) : 
    LinkedInRequestStrategy(networkManager,
                            RequestsPerSecond,
                            RequestCooldownMs,
                            MaxConcurrentRequests,
                            parent)
{
}

LinkedInRequestStrategy::LinkedInRequestStrategy(AbstractNetworkManager* const networkManager, 
                                                 size_t requestPerSecond, 
                                                 size_t requestCooldown, 
                                                 size_t maxConcurrent, 
                                                 QObject* parent) :
    AbstractRequestStrategy(networkManager, parent),
    m_activeReplies(),
    m_summaryUrlsQueue(),
    m_detailsUrlsQueue(),
    m_requestTimer(this),
    m_requestCooldownTimer(this),
    m_requestsPerSecond(requestPerSecond),
    m_requestIntervalMs(MillisecondsPerSecond / m_requestsPerSecond),
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

    if (!m_vacancyCountDetermined && m_summaryUrlsQueue.isEmpty())
    {
        m_vacancyCountDetermined = true;
        emit vacancyCountDetermined(m_detailsUrlsQueue.size());
    }

    const unsigned _maxConcurrent = m_cookiesInitialized ? m_maxConcurrent : InitialConcurrencyLimit;

    if (hasPendingRequests() && m_activeReplies.size() < _maxConcurrent)
    {
        emit requestsStarted();
        emit urlsDequeued();

        QUrl url;
        if (!m_summaryUrlsQueue.isEmpty())
        {
            url = m_summaryUrlsQueue.dequeue();
        }
        else
        {
            url = m_detailsUrlsQueue.dequeue();
        }

        
        qCDebug(LinkedInRequestStrategyLog) << "Requesting URL:" << url;

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
    AbstractRequestStrategy::interrupt();

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
    const QString errorMessage = reply->errorString();
    const QString html = reply->readAll();
    const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    qCDebug(LinkedInRequestStrategyLog) << "Received response for URL:" << url;

    reply->deleteLater();

    if (errorState != QNetworkReply::NoError)
    {

        switch (statusCode)
        {
            case TooManyRequestsStatusCode:
            {
                qCWarning(LinkedInRequestStrategyLog) << "Request was rate limited; retrying after" << m_requestCooldown << "ms:" << url;
                m_requestCooldownTimer.start(m_requestCooldown);
                addUrl(url);
                break;
            }
            default:
            {
                qCWarning(LinkedInRequestStrategyLog) << "Request failed for" << url << "with network error" << errorState << errorMessage << "and HTTP status" << statusCode;
                break;
            }
        }

        emit meaninglessRepliesReceived();
        return;
    }


    initCookies(url);

    if (updateSummaryUrlCutoff(html, url))
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
        case UrlType::Summary:
        {
            m_summaryUrlsQueue.enqueue(url);
            return true;
        }
        case UrlType::Details:
        {
            m_detailsUrlsQueue.enqueue(url);
            return true;
        }
        default:
        {
            qCWarning(LinkedInRequestStrategyLog) << "Ignoring unsupported URL:" << url;
            return false;
        }
    }
}

void LinkedInRequestStrategy::clearQueues()
{
    if (!m_summaryUrlsQueue.isEmpty())
    {
        emit urlsDequeued(m_summaryUrlsQueue.size());
        m_summaryUrlsQueue.clear();
    }

    if (!m_detailsUrlsQueue.isEmpty())
    {
        emit urlsDequeued(m_detailsUrlsQueue.size());
        m_detailsUrlsQueue.clear();
    }
}

bool LinkedInRequestStrategy::hasPendingRequests() const
{
    return !m_summaryUrlsQueue.isEmpty() || !m_detailsUrlsQueue.isEmpty();
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
        }
    }
}

bool LinkedInRequestStrategy::updateSummaryUrlCutoff(const QString& html, const QUrl& url)
{
    if (clasifyUrl(url) != UrlType::Summary)
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

    const qsizetype removed = m_summaryUrlsQueue.removeIf(isAfterStartPred);
    emit urlsDequeued(removed);

    qCDebug(LinkedInRequestStrategyLog) << "Removed" << removed << "later result pages after an empty page at offset" << start;

    return true;
}

UrlType clasifyUrl(const QUrl& url)
{
    if (url.hasQuery())
        return UrlType::Summary;

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
