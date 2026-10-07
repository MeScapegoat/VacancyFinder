#pragma once

#include <QNetworkReply>

class FakeNetworkReply final : public QNetworkReply
{
public:
    explicit FakeNetworkReply(const QNetworkRequest& request, 
                              int statusCode, 
                              const QByteArray& data, 
                              QNetworkReply::NetworkError error = QNetworkReply::NoError,
                              QObject* parent = nullptr);

    void abort() override;

    qint64 size() const override;

    bool isSequential() const override;

protected:
    // supports only readAll();
    qint64 readData(char* data, qint64 maxSize) override;

private:
    QByteArray m_data;
};