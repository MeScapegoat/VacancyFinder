// std

// Qt
#include <QThread>

// custom
#include "NetworkManager.h"

namespace Linked
{
    void NetworkManager::sendRequest(QUrl link)
    {
        qInfo() << "SendRequest:" << link;

        request.setUrl(link);
        QThread::msleep(500);
        get(request);
    }

} // namespace Linked