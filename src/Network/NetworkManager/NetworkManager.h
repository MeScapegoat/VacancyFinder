#pragma once

#include "AbstractNetworkManager.h"

class QNetworkReply;
class QNetworkRequest;
class QNetworkCookieJar;
class QNetworkAccessManager;

class NetworkManager final : public AbstractNetworkManager
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject* parent = nullptr);
    ~NetworkManager() = default;

public:
    QNetworkCookieJar* cookieJar() override;
    const QNetworkCookieJar* cookieJar() const override;
    void setCookieJar(QNetworkCookieJar* cookieJar) override;
    QNetworkReply* get(const QNetworkRequest& request) override;

private:
    QNetworkAccessManager& m_networkManager;
};
