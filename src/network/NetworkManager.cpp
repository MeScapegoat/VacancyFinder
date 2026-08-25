// std

// Qt
#include <QThread>
#include <QNetworkAccessManager>

// custom
#include "NetworkManager.h"

namespace Linked
{
    Q_LOGGING_CATEGORY(LinkedNetwork, "network");

    NetworkManager::NetworkManager(SharedContext* sharedContext, QObject* parent) :
        QObject(parent),
        context(sharedContext),
        networkManager(*new QNetworkAccessManager(this)),
        urlsQueue(),
        activeReplies()
    {
        QObject::connect(&networkManager, &QNetworkAccessManager::finished, this, &NetworkManager::onFinished);
    }

    void NetworkManager::addUrls(const QList<QUrl>& urls)
    {
        for (const QUrl& url : urls)
        {
            urlsQueue.enqueue(url);
        }

        startNext();
    }

    void NetworkManager::addUrl(const QUrl& url)
    {
        urlsQueue.enqueue(url);

        startNext();
    }

    void NetworkManager::startNext()
    {
        while (!urlsQueue.isEmpty() && activeReplies.size() < MaxConcurrent)
        {
            const QUrl url = urlsQueue.dequeue();
            qCDebug(LinkedNetwork) << url;

            QNetworkRequest request(url);

            request.setRawHeader("Accept-Language", "en-US,en;q=0.9");

            //request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0");
            //QThread::msleep(500);
            QNetworkReply* reply = networkManager.get(request);

            activeReplies.insert(reply);
        }
    }

    void NetworkManager::onFinished(QNetworkReply* reply)
    {
        activeReplies.remove(reply);

        //const QNetworkReply::NetworkError error = reply->error();
        //if (error == QNetworkReply::NoError)
        //{
        //    const QByteArray html = reply->readAll();
        //
        //    processHtml(reply->url(), html);
        //}
        //else
        //{
        //    handleError(reply);
        //}

        emit receivedReply(reply);

        startNext();
    }

} // namespace Linked