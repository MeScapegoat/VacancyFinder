#pragma once

#include <QObject>

#include <QLoggingCategory>
#include <QUrl>


Q_DECLARE_LOGGING_CATEGORY(RequestManagerLog);


class AbstractRequestStrategy;

class RequestManager : public QObject
{
    Q_OBJECT

public:
    explicit RequestManager(AbstractRequestStrategy* const strategy, QObject* parent = nullptr);
    ~RequestManager() = default;

public:
    const AbstractRequestStrategy* const strategy() const;

public slots:
    void addUrls(const QList<QUrl>& urls);
    void addUrl(const QUrl& url);

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
    AbstractRequestStrategy* m_strategy;
};
