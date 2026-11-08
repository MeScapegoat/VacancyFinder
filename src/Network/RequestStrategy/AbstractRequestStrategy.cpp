#include "AbstractRequestStrategy.h"

#include <QNetworkReply>
#include <QNetworkRequest>


Q_LOGGING_CATEGORY(RequestStrategyLog, "network.requestStrategy");

AbstractRequestStrategy::AbstractRequestStrategy(AbstractNetworkManager* const networkManager, QObject* parent) :
    QObject(parent),
    m_networkManager(networkManager),
    m_isInterrupted(false)
{
    Q_ASSERT_X(m_networkManager, __FUNCTION__, "NetworkManager is nullptr");

    m_networkManager->setParent(this);
    QObject::connect(m_networkManager, &AbstractNetworkManager::finished, this, &AbstractRequestStrategy::handleReceivedReply);
}

void AbstractRequestStrategy::reset()
{
    m_isInterrupted = false;
}

void AbstractRequestStrategy::interrupt()
{
    m_isInterrupted = true;
}

UrlType AbstractRequestStrategy::classifyUrl(const QUrl& url) const
{
    if (url.hasQuery())
        return UrlType::Summary;

    return UrlType::Details;
}

bool AbstractRequestStrategy::handleNetworkError(QNetworkReply* reply)
{
    const QNetworkReply::NetworkError errorState = reply->error();

    if (errorState != QNetworkReply::NoError)
    {
        handleHttpStatusCode(reply);

        emit meaninglessRepliesReceived();
        return true;
    }
    return false;
}

void AbstractRequestStrategy::handleHttpStatusCode(QNetworkReply* reply)
{
    const QNetworkReply::NetworkError errorState = reply->error();
    const QUrl url = reply->url();
    const QString errorMessage = reply->errorString();
    const int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    qCWarning(RequestStrategyLog) << "Request failed for" << url << "with network error" << errorState << errorMessage << "and HTTP status" << statusCode;
}
