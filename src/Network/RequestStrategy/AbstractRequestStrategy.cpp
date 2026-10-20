#include "AbstractRequestStrategy.h"


AbstractRequestStrategy::AbstractRequestStrategy(AbstractNetworkManager* const networkManager, QObject* parent) :
    QObject(parent),
    m_networkManager(networkManager),
    m_isInterrupted(false)
{
    Q_ASSERT_X(m_networkManager, __FUNCTION__, "NetworkManager is nullptr");

    m_networkManager->setParent(this);
    QObject::connect(m_networkManager, &AbstractNetworkManager::finished, this, &AbstractRequestStrategy::handleReceivedReply);
}

void AbstractRequestStrategy::reset()
{
    m_isInterrupted = false;
}

void AbstractRequestStrategy::interrupt()
{
    m_isInterrupted = true;
}