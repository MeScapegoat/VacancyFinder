#pragma once

#include <QObject>

#include <QLoggingCategory>
#include <QUrl>

#include <ConnectionGroup.h>


Q_DECLARE_LOGGING_CATEGORY(RequestManagerLog);


class AbstractRequestStrategy;

class RequestManager : public QObject
{
    Q_OBJECT

public:
    explicit RequestManager(AbstractRequestStrategy* const strategy, QObject* parent = nullptr);
    ~RequestManager() = default;

public:
    void setStrategy(AbstractRequestStrategy* const strategy);
    const AbstractRequestStrategy* const strategy() const;

public:
    void reset();

public slots:
    void addUrls(const QList<QUrl>& urls);
    void addUrl(const QUrl& url);

    void interrupt();

signals:
    void urlsEnqueued(int amount = 1);
    void urlsDequeued(int amount = 1);

    void requestsStarted(int amount = 1);
    void requestsFinished(int amount = 1);

    void meaningfulRepliesReceived(int amount = 1);
    void meaninglessRepliesReceived(int amount = 1);

    void vacancyCountDetermined(int count);

    void receivedReply(const QString& html, const QUrl& url);

protected:
    void connectStrategy();
    void disconnectStrategy();

private:
    AbstractRequestStrategy* m_strategy;
    ConnectionGroup connections;
};
