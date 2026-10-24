#pragma once

#include "AbstractNetworkManager.h"

#include <QList>
#include <QNetworkCookieJar>
#include <QNetworkRequest>
#include <QNetworkReply>

class FakeNetworkManager final : public AbstractNetworkManager
{
public:
    explicit FakeNetworkManager(QObject* parent = nullptr);
    ~FakeNetworkManager() = default;

public:
    QNetworkCookieJar* cookieJar() override;
    const QNetworkCookieJar* cookieJar() const override;
    void setCookieJar(QNetworkCookieJar* cookieJar) override;
    QNetworkReply* get(const QNetworkRequest& request) override;

    void respondWith(int statusCode,
                 const QByteArray& data,
                 QNetworkReply::NetworkError error = QNetworkReply::NoError);

    void finish();
private:
    int m_statusCode;
    QByteArray m_data;
    QNetworkReply::NetworkError m_error;

    QNetworkCookieJar* m_cookieJar = nullptr;
    QNetworkReply* m_lastReply;
};
