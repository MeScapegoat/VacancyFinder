#pragma once

#include <QObject>

class QNetworkReply;
class QNetworkRequest;
class QNetworkCookieJar;

class AbstractNetworkManager : public QObject
{
    Q_OBJECT

public:
    explicit AbstractNetworkManager(QObject* parent = nullptr) :
        QObject(parent)
    {
    }
    virtual ~AbstractNetworkManager() = default;

public:
    virtual QNetworkCookieJar* cookieJar() const = 0;
    virtual void setCookieJar(QNetworkCookieJar* cookieJar) = 0;
    virtual QNetworkReply* get(const QNetworkRequest& request) = 0;

signals:
    void finished(QNetworkReply* reply);
};