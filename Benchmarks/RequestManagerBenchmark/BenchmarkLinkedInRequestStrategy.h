#pragma once

#include <QElapsedTimer>
#include <QMap>
#include <QtGlobal>
#include <QTimer>

#include "LinkedInRequestStrategy.h"

struct BenchmarkRequestStatistics
{
    qint64 successfulResponses;
    qint64 rateLimitedResponses;
    qint64 rateLimitedResponsesWithRetryAfter;
    qint64 rateLimitedResponsesWithoutRetryAfter;
    QMap<QString, qint64> retryAfterHeaderCounts;
    qint64 failedResponses;
    qint64 emptySummaryResponses;
    qint64 totalCooldownTimeMs;
};

class BenchmarkLinkedInRequestStrategy : public LinkedInRequestStrategy
{
    Q_OBJECT

public:
    explicit BenchmarkLinkedInRequestStrategy(AbstractNetworkManager* const networkManager, QObject* parent = nullptr);
    explicit BenchmarkLinkedInRequestStrategy(AbstractNetworkManager* const networkManager, 
                                     size_t requestPerSecond,
                                     size_t requestCooldown,
                                     size_t maxConcurrent,
                                     QObject* parent = nullptr);
    ~BenchmarkLinkedInRequestStrategy() = default;

    BenchmarkRequestStatistics statistics() const;

public:
    void reset() override;

public slots:
    void interrupt() override;

protected:
    bool handleNetworkError(QNetworkReply* reply) override;

private:
    void finishCooldownMeasurement();

private:
    QTimer m_cooldownMeasurementTimer;
    QElapsedTimer m_cooldownElapsedTimer;
    QMap<QString, qint64> m_retryAfterHeaderCounts;

    qint64 m_successfulResponses = 0;
    qint64 m_rateLimitedResponses = 0;
    qint64 m_rateLimitedResponsesWithRetryAfter = 0;
    qint64 m_rateLimitedResponsesWithoutRetryAfter = 0;
    qint64 m_failedResponses = 0;
    qint64 m_emptySummaryResponses = 0;
    qint64 m_totalCooldownTimeMs = 0;

    size_t m_requestCooldown;
};

