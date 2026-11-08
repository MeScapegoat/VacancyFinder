#pragma once

#include <QList>
#include <QLoggingCategory>
#include <QObject>
#include <QUrl>

#include <AbstractNetworkManager.h>

enum class UrlType
{
    Unknown = 0,
    Summary,
    Details
};

Q_DECLARE_LOGGING_CATEGORY(RequestStrategyLog);

class QNetworkReply;

class AbstractRequestStrategy : public QObject
{
    Q_OBJECT
public:
    explicit AbstractRequestStrategy(AbstractNetworkManager* const networkManager, QObject* parent = nullptr);
    virtual ~AbstractRequestStrategy() = default;

public:
    virtual void startNext() = 0;
    virtual UrlType classifyUrl(const QUrl& url) const;

public slots:
    virtual void reset();

    virtual void addUrls(const QList<QUrl>& urls) = 0;
    virtual void addUrl(const QUrl& url) = 0;

    virtual void interrupt();

    virtual void handleReceivedReply(QNetworkReply* reply) = 0;

signals:
    void urlsEnqueued(int amount = 1);
    void urlsDequeued(int amount = 1);

    void requestsStarted(int amount = 1);
    void requestsFinished(int amount = 1);

    void meaningfulRepliesReceived(int amount = 1);
    void meaninglessRepliesReceived(int amount = 1);

    void summaryRequestsFinished();

    void receivedReply(const QString& html, const QUrl& url);

protected:
    virtual bool handleNetworkError(QNetworkReply* reply);
    virtual void handleHttpStatusCode(QNetworkReply* reply);

    bool m_isInterrupted;
    AbstractNetworkManager* m_networkManager;
};
