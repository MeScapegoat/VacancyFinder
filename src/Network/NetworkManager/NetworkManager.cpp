#include "NetworkManager.h"

#include <QNetworkAccessManager>

NetworkManager::NetworkManager(QObject* parent) :
    AbstractNetworkManager(parent),
    m_networkManager(*new QNetworkAccessManager(this))
{
    QObject::connect(&m_networkManager, &QNetworkAccessManager::finished, this, &NetworkManager::finished);
}

QNetworkCookieJar* NetworkManager::cookieJar()
{
    return m_networkManager.cookieJar();
}

const QNetworkCookieJar* NetworkManager::cookieJar() const
{
    return m_networkManager.cookieJar();
}

void NetworkManager::setCookieJar(QNetworkCookieJar* cookieJar)
{
    m_networkManager.setCookieJar(cookieJar);
}

QNetworkReply* NetworkManager::get(const QNetworkRequest& request)
{
    return m_networkManager.get(request);
}
