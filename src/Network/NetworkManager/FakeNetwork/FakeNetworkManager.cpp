#include "FakeNetworkManager.h"

#include "FakeNetworkReply.h"

FakeNetworkManager::FakeNetworkManager(QObject* parent) :
    AbstractNetworkManager(parent),
    m_statusCode(),
    m_data(),
    m_error(),
    m_cookieJar(new QNetworkCookieJar(this)),
    m_lastReply(nullptr)
{}

QNetworkCookieJar* FakeNetworkManager::cookieJar()
{
    return m_cookieJar;
}

const QNetworkCookieJar* FakeNetworkManager::cookieJar() const
{
    return m_cookieJar;
}

void FakeNetworkManager::setCookieJar(QNetworkCookieJar* cookieJar)
{
    if (cookieJar == m_cookieJar)
        return;

    m_cookieJar->deleteLater();
    cookieJar->setParent(this);

    m_cookieJar = cookieJar;
}

QNetworkReply* FakeNetworkManager::get(const QNetworkRequest& request)
{
    m_lastReply = new FakeNetworkReply(request, m_statusCode, m_data, m_error);

    return m_lastReply;
}

void FakeNetworkManager::respondWith(int statusCode, const QByteArray& data, QNetworkReply::NetworkError error)
{
    m_statusCode = statusCode;
    m_data = data;
    m_error = error;
}


void FakeNetworkManager::finish()
{
    if (m_lastReply == nullptr)
    {
        return;
    }

    emit finished(m_lastReply);
}
