#pragma once

#include <QList>
#include <QObject>
#include <QUrl>

class AbstractRequestStrategy;

class RequestManager : public QObject
{
    Q_OBJECT

public:
    // Takes ownership of strategy.
    explicit RequestManager(AbstractRequestStrategy& strategy, QObject* parent = nullptr);
    ~RequestManager() = default;

public:
    const AbstractRequestStrategy& strategy() const;

public slots:
    virtual void addUrls(const QList<QUrl>& urls);
    virtual void addUrl(const QUrl& url);

signals:
    void reset();
    void interrupt();

    void urlsEnqueued(int amount = 1);
    void urlsDequeued(int amount = 1);

    void requestsStarted(int amount = 1);
    void requestsFinished(int amount = 1);

    void meaningfulRepliesReceived(int amount = 1);
    void meaninglessRepliesReceived(int amount = 1);

    void vacancyCountDetermined(int count);

    void receivedReply(const QString& html, const QUrl& url);

private:
    AbstractRequestStrategy& m_strategy;
};
