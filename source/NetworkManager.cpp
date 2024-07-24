// std

// Qt
#include <QThread>

// custom
#include "NetworkManager.h"

namespace Linked
{
    Q_LOGGING_CATEGORY(LinkedNetwork, "network");

    NetworkManager::NetworkManager()
    {
        qCDebug(LinkedNetwork) << QThread::currentThread();
    }

    NetworkManager::~NetworkManager()
    {
        qCDebug(LinkedNetwork) << QThread::currentThread();
    }

    void NetworkManager::sendRequest(QUrl link)
    {
        qCDebug(LinkedNetwork) << link;

        request.setUrl(link);
        QThread::msleep(500);
        get(request);
    }

} // namespace Linked