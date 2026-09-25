#pragma once

#include <QObject>

#include <QLoggingCategory>
#include <QQueue>
#include <QSet>
#include <QTimer>
#include <QNetworkReply>

Q_DECLARE_LOGGING_CATEGORY(NetworkLog);

class SharedContext;

class RequestManager : public QObject
{
    Q_OBJECT

public:
    RequestManager(QObject* parent = nullptr);
    ~RequestManager() = default;

public:
    void setRequestsPerSecond(unsigned int requestsPerSecond);
    void setMaxConcurrent(const unsigned int maxConcurrent);
    void setRequestCooldownMs(const unsigned int cooldown);

    quint64 getErrorCount() const;

    void reset();

public slots:
    void addUrls(const QList<QUrl>& urls);
    void addUrl(const QUrl& url);

    void onInterrupt();

signals:
    void receivedReply(const QString& html, const QUrl& url);
    void replyError(const QNetworkReply::NetworkError& error);
    void requestsFinished();

protected slots:
    void onFinished(QNetworkReply* reply);

protected:
    void startNext();
    void cancelAll();

    void stopTimers();
    void cancelReplies();
    void clearQueue();

private:
    QNetworkAccessManager& networkManager;

    QQueue<QUrl> urlsQueue;
    QSet<QNetworkReply*> activeReplies;

    bool cookiesInitialized;
    bool isInterrupted;


    QTimer requestTimer;
    unsigned requestsPerSecond;
    unsigned requestIntervalMs;

    QTimer requestCooldownTimer;
    unsigned requestCooldown;

    unsigned maxConcurrent;

    quint64 errorCount = 0;
};
