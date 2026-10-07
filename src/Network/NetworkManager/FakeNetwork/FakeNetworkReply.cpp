#include "FakeNetworkReply.h"


FakeNetworkReply::FakeNetworkReply(const QNetworkRequest& request, 
                                   int statusCode, 
                                   const QByteArray& data, 
                                   QNetworkReply::NetworkError error, 
                                   QObject* parent) :
    QNetworkReply(parent),
    m_data(data)
{
    setRequest(request);
    setUrl(request.url());
    setOpenMode(QIODevice::ReadOnly);

    setAttribute(QNetworkRequest::HttpStatusCodeAttribute, statusCode);
    setError(error, "");
}

void FakeNetworkReply::abort()
{
    setError(QNetworkReply::OperationCanceledError,
             QStringLiteral("Fake network reply aborted"));

    emit finished();
}

qint64 FakeNetworkReply::size() const
{
    return m_data.size();
}

bool FakeNetworkReply::isSequential() const
{
    return false;
}

qint64 FakeNetworkReply::readData(char* data, qint64 maxSize)
{
    const qint64 size = qMin(maxSize, static_cast<qint64>(m_data.size()));

    if (size == 0)
        return 0;

    memcpy(data, m_data.constData(), static_cast<size_t>(size));

    return size;
}