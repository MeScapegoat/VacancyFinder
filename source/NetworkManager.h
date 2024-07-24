#pragma once
// std

// Qt
#include <QNetworkAccessManager>
#include <QLoggingCategory>

// custom

namespace Linked
{
    Q_DECLARE_LOGGING_CATEGORY(LinkedNetwork);

    class NetworkManager : public QNetworkAccessManager
    {
        Q_OBJECT

    public:
        NetworkManager();
        ~NetworkManager();

    public slots:
        void sendRequest(QUrl);

    signals:
        void titleFinished();

    private:
        QNetworkRequest request;
    };

} // namespace Linked
