#include "RequestManager.h"

#include "RequestStrategy/AbstractRequestStrategy.h"


Q_LOGGING_CATEGORY(RequestManagerLog, "network.requestManager");

RequestManager::RequestManager(AbstractRequestStrategy* const strategy, QObject* parent) :
    QObject(parent),
    m_strategy(strategy),
    connections()
{
    Q_ASSERT_X(strategy, __FUNCTION__, "Strategy is nullptr");
    Q_ASSERT_X(strategy->thread() == thread(), __FUNCTION__, "Strategy must belong to the same thread as RequestManager");

    connectStrategy();
}

void RequestManager::setStrategy(AbstractRequestStrategy* const strategy)
{
    Q_ASSERT_X(strategy, __FUNCTION__, "Strategy is nullptr");
    Q_ASSERT_X(strategy->thread() == thread(), __FUNCTION__, "Strategy must belong to the same thread as RequestManager");

    if (m_strategy == strategy)
        return;

    qCInfo(RequestManagerLog) << "New RequestStrategy:" << strategy;

    disconnectStrategy();

    m_strategy = strategy;

    connectStrategy();
}

const AbstractRequestStrategy* const RequestManager::strategy() const
{
    return m_strategy;
}

void RequestManager::reset()
{
    m_strategy->reset();
}

void RequestManager::addUrls(const QList<QUrl>& urls)
{
    m_strategy->addUrls(urls);
}

void RequestManager::addUrl(const QUrl& url)
{
    m_strategy->addUrl(url);
}

void RequestManager::interrupt()
{
    m_strategy->interrupt();
}

void RequestManager::connectStrategy()
{
    connections += QObject::connect(m_strategy, &AbstractRequestStrategy::urlsEnqueued, this, &RequestManager::urlsEnqueued);
    connections += QObject::connect(m_strategy, &AbstractRequestStrategy::urlsDequeued, this, &RequestManager::urlsDequeued);

    connections += QObject::connect(m_strategy, &AbstractRequestStrategy::requestsStarted, this, &RequestManager::requestsStarted);
    connections += QObject::connect(m_strategy, &AbstractRequestStrategy::requestsFinished, this, &RequestManager::requestsFinished);

    connections += QObject::connect(m_strategy, &AbstractRequestStrategy::meaningfulRepliesReceived, this, &RequestManager::meaningfulRepliesReceived);
    connections += QObject::connect(m_strategy, &AbstractRequestStrategy::meaninglessRepliesReceived, this, &RequestManager::meaninglessRepliesReceived);

    connections += QObject::connect(m_strategy, &AbstractRequestStrategy::vacancyCountDetermined, this, &RequestManager::vacancyCountDetermined);

    connections += QObject::connect(m_strategy, &AbstractRequestStrategy::receivedReply, this, &RequestManager::receivedReply);
}

void RequestManager::disconnectStrategy()
{
    connections.disconnect();
}