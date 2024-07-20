#pragma once
#include <QNetworkAccessManager>

namespace Linked
{
    class NetworkManager : public QNetworkAccessManager
    {
        Q_OBJECT

    public:
        NetworkManager() = default;
        ~NetworkManager() = default;

    public slots:
        void sendRequest(QUrl link);

    signals:
        void titleFinished();

    private:
        QNetworkRequest request;
    };

} // namespace Linked
