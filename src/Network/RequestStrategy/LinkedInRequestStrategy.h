#pragma once

#include "AbstractRequestStrategy.h"

#include <QLoggingCategory>
#include <QNetworkAccessManager>
#include <QQueue>
#include <QSet>
#include <QTimer>
#include <QUrl>

Q_DECLARE_LOGGING_CATEGORY(LinkedInRequestStrategyLog);

enum class UrlType
{
    Unknown = 0,
    Brief,
    Details
};

class QNetworkReply;

class LinkedInRequestStrategy : public AbstractRequestStrategy
{
    Q_OBJECT

public:
    explicit LinkedInRequestStrategy(AbstractNetworkManager* const networkManager, QObject* parent = nullptr);
    explicit LinkedInRequestStrategy(AbstractNetworkManager* const networkManager, 
                                     size_t requestPerSecond,
                                     size_t requestCooldown,
                                     size_t maxConcurrent,
                                     QObject* parent = nullptr);
    ~LinkedInRequestStrategy() = default;

public:
    void reset() override;
    void startNext() override;

public slots:
    void addUrls(const QList<QUrl>& urls) override;
    void addUrl(const QUrl& url) override;

    void interrupt() override;

    void handleReceivedReply(QNetworkReply* reply) override;

private:
    void init();

    bool enqueueUrl(const QUrl& url);
    void clearQueues();
    bool hasPendingRequests() const;

    void cancelAll();
    void cancelReplies();
    void stopTimers();
    void startRequestTimer();
    
    void initCookies(const QUrl& url);

private:
    // Optimization:
    // LinkedIn may return meaningful replies up to a certain start, followed by
    // empty HTML responses. Once the cutoff is detected, remove the later URLs
    // to avoid processing them unnecessarily.
    bool updateBriefUrlCutoff(const QString& html, const QUrl& url);

private:
    QSet<QNetworkReply*> m_activeReplies;

    QQueue<QUrl> m_detailsUrlsQueue;
    QQueue<QUrl> m_briefUrlsQueue;

    QTimer m_requestTimer;
    QTimer m_requestCooldownTimer;

    size_t m_requestsPerSecond;
    size_t m_requestIntervalMs;
    size_t m_requestCooldown;
    size_t m_maxConcurrent;

    bool m_cookiesInitialized;
    std::optional<int> m_minStart;
    bool m_vacancyCountDetermined;
};


UrlType clasifyUrl(const QUrl& url);
int getStart(const QUrl& url);
bool isAfterStart(int start, const QUrl& url);
