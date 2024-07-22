// std

// Qt
#include <QThread>

// custom
#include "NetworkManager.h"

Q_LOGGING_CATEGORY(LinkedNetwork, "Network");

namespace Linked
{
    void NetworkManager::sendRequest(QUrl link)
    {
        qCDebug(LinkedNetwork) << link;

        request.setUrl(link);
        QThread::msleep(500);
        get(request);
    }

} // namespace Linked