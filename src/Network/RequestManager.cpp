#include "RequestManager.h"

#include "RequestStrategy/AbstractRequestStrategy.h"

RequestManager::RequestManager(AbstractRequestStrategy& strategy, QObject* parent) :
    QObject(parent),
    m_strategy(strategy)
{
    Q_ASSERT_X(strategy.thread() == thread(), __FUNCTION__, "Strategy must belong to the same thread as RequestManager");

    strategy.setParent(this);

    QObject::connect(this, &RequestManager::reset, &m_strategy, &AbstractRequestStrategy::reset);
    QObject::connect(this, &RequestManager::interrupt, &m_strategy, &AbstractRequestStrategy::interrupt);

    QObject::connect(&m_strategy, &AbstractRequestStrategy::urlsEnqueued, this, &RequestManager::urlsEnqueued);
    QObject::connect(&m_strategy, &AbstractRequestStrategy::urlsDequeued, this, &RequestManager::urlsDequeued);

    QObject::connect(&m_strategy, &AbstractRequestStrategy::requestsStarted, this, &RequestManager::requestsStarted);
    QObject::connect(&m_strategy, &AbstractRequestStrategy::requestsFinished, this, &RequestManager::requestsFinished);

    QObject::connect(&m_strategy, &AbstractRequestStrategy::meaningfulRepliesReceived, this, &RequestManager::meaningfulRepliesReceived);
    QObject::connect(&m_strategy, &AbstractRequestStrategy::meaninglessRepliesReceived, this, &RequestManager::meaninglessRepliesReceived);

    QObject::connect(&m_strategy, &AbstractRequestStrategy::vacancyCountDetermined, this, &RequestManager::vacancyCountDetermined);

    QObject::connect(&m_strategy, &AbstractRequestStrategy::receivedReply, this, &RequestManager::receivedReply);
}

const AbstractRequestStrategy& RequestManager::strategy() const
{
    return m_strategy;
}

void RequestManager::addUrls(const QList<QUrl>& urls)
{
    m_strategy.addUrls(urls);
}

void RequestManager::addUrl(const QUrl& url)
{
    m_strategy.addUrl(url);
}
