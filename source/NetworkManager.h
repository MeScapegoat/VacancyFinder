#pragma once
// std

// Qt
#include <QNetworkAccessManager>
#include <QLoggingCategory>

// custom

Q_DECLARE_LOGGING_CATEGORY(LinkedNetwork);

namespace Linked
{
    class NetworkManager : public QNetworkAccessManager
    {
        Q_OBJECT

    public:
        NetworkManager() = default;
        ~NetworkManager() = default;

    public slots:
        void sendRequest(QUrl);

    signals:
        void titleFinished();

    private:
        QNetworkRequest request;
    };

} // namespace Linked
