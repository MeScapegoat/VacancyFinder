#include "NetworkManager.h"

namespace Linked
{
    void NetworkManager::sendRequest(QUrl link)
    {
        qInfo() << "SendRequest: " << link;

        request.setUrl(link);
        get(request);
    }

} // namespace Linked