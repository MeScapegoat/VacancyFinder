#pragma once
// std

// Qt
#include <QNetworkAccessManager>
#include <QLoggingCategory>

// custom
#include "../core/SharedContext.h"

namespace Linked
{
    Q_DECLARE_LOGGING_CATEGORY(LinkedNetwork);

    class SharedContext;

    class NetworkManager : public QNetworkAccessManager
    {
        Q_OBJECT

    public:
        NetworkManager(Linked::SharedContext *);
        ~NetworkManager() = default;

    public slots:
        void sendRequest(QUrl);

    signals:
        void titleFinished();

    private:
        Linked::SharedContext *context = nullptr;

    private:
        QNetworkRequest request;
    };

} // namespace Linked
