#pragma once
// std

// Qt
#include <QLoggingCategory>
#include <QObject>
#include <QQueue>
#include <QSet>

// custom
#include "../core/SharedContext.h"

class QNetworkAccessManager;
class QNetworkReply;

namespace Linked
{
    Q_DECLARE_LOGGING_CATEGORY(LinkedNetwork);

    class SharedContext;

    class NetworkManager : public QObject
    {
        Q_OBJECT

    public:
        NetworkManager(Linked::SharedContext* context, QObject* parent = nullptr);
        ~NetworkManager() = default;

    public slots:
        void addUrls(const QList<QUrl>& urls);
        void addUrl(const QUrl& url);
        

    signals:
        void receivedReply(QNetworkReply* reply);

    protected slots:
        void onFinished(QNetworkReply* reply);

    protected:
        void startNext();

    private:
        QNetworkAccessManager& networkManager;

        QQueue<QUrl> urlsQueue;
        QSet<QNetworkReply*> activeReplies;

        static constexpr int MaxConcurrent = 10;

    private:
        Linked::SharedContext *context = nullptr;
    };

} // namespace Linked
