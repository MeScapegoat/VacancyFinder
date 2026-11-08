#include "BenchmarkLinkedInRequestStrategy.h"

#include <QByteArray>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QString>
#include <QUrl>

namespace
{
    constexpr qsizetype RequestsPerSecond = 8;
    constexpr qsizetype RequestCooldownMs = 5000;
    constexpr qsizetype MaxConcurrentRequests = 8;
    constexpr qsizetype TooManyRequestsStatusCode = 429;
}

BenchmarkLinkedInRequestStrategy::BenchmarkLinkedInRequestStrategy(AbstractNetworkManager* const networkManager, QObject* parent) :
    BenchmarkLinkedInRequestStrategy(networkManager,
                                     RequestsPerSecond,
                                     RequestCooldownMs,
                                     MaxConcurrentRequests,
                                     parent)
{
}

BenchmarkLinkedInRequestStrategy::BenchmarkLinkedInRequestStrategy(AbstractNetworkManager* const networkManager,
                                                                   size_t requestPerSecond,
                                                                   size_t requestCooldown,
                                                                   size_t maxConcurrent,
                                                                   QObject* parent) :
    LinkedInRequestStrategy(networkManager, requestPerSecond, requestCooldown, maxConcurrent, parent),
    m_cooldownMeasurementTimer(this),
    m_cooldownElapsedTimer(),
    m_requestCooldown(requestCooldown)
{
    m_cooldownMeasurementTimer.setSingleShot(true);
    connect(&m_cooldownMeasurementTimer, &QTimer::timeout, this, &BenchmarkLinkedInRequestStrategy::finishCooldownMeasurement);
}

BenchmarkRequestStatistics BenchmarkLinkedInRequestStrategy::statistics() const
{
    return {
        m_successfulResponses,
        m_rateLimitedResponses,
        m_rateLimitedResponsesWithRetryAfter,
        m_rateLimitedResponsesWithoutRetryAfter,
        m_retryAfterHeaderCounts,
        m_failedResponses,
        m_emptySummaryResponses,
        m_totalCooldownTimeMs
    };
}

void BenchmarkLinkedInRequestStrategy::reset()
{
    LinkedInRequestStrategy::reset();

    m_successfulResponses = 0;
    m_rateLimitedResponses = 0;
    m_rateLimitedResponsesWithRetryAfter = 0;
    m_rateLimitedResponsesWithoutRetryAfter = 0;
    m_failedResponses = 0;
    m_emptySummaryResponses = 0;
    m_totalCooldownTimeMs = 0;
    m_retryAfterHeaderCounts.clear();
}

void BenchmarkLinkedInRequestStrategy::interrupt()
{
    LinkedInRequestStrategy::interrupt();

    m_cooldownMeasurementTimer.stop();
    finishCooldownMeasurement();
}

bool BenchmarkLinkedInRequestStrategy::handleNetworkError(QNetworkReply* reply)
{
    const QNetworkReply::NetworkError errorState = reply->error();
    const QUrl url = reply->url();
    const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    const QByteArray retryAfterHeader = reply->rawHeader("Retry-After");
    const QByteArray html = reply->peek(reply->bytesAvailable());

    if (statusCode == TooManyRequestsStatusCode)
    {
        ++m_rateLimitedResponses;
        if (retryAfterHeader.isEmpty())
        {
            ++m_rateLimitedResponsesWithoutRetryAfter;
        }
        else
        {
            ++m_rateLimitedResponsesWithRetryAfter;
            const QString retryAfterHeaderValue = QString::fromLatin1(retryAfterHeader);
            ++m_retryAfterHeaderCounts[retryAfterHeaderValue];
        }

        if (errorState != QNetworkReply::NoError)
        {
            if (!m_cooldownMeasurementTimer.isActive())
                m_cooldownElapsedTimer.start();
            m_cooldownMeasurementTimer.start(m_requestCooldown);
        }
    }
    else if (errorState != QNetworkReply::NoError)
    {
        ++m_failedResponses;
    }
    else
    {
        ++m_successfulResponses;
    }

    if (errorState == QNetworkReply::NoError &&
        classifyUrl(url) == UrlType::Summary &&
        !html.contains("<li>"))
    {
        ++m_emptySummaryResponses;
    }

    return LinkedInRequestStrategy::handleNetworkError(reply);
}

void BenchmarkLinkedInRequestStrategy::finishCooldownMeasurement()
{
    if (m_cooldownElapsedTimer.isValid())
    {
        m_totalCooldownTimeMs += m_cooldownElapsedTimer.elapsed();
        m_cooldownElapsedTimer.invalidate();
    }
}
