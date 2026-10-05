#pragma once

#include <QList>
#include <QMetaObject>

class ConnectionGroup
{
public:
    ConnectionGroup() = default;
    ~ConnectionGroup();

public:
    ConnectionGroup& operator+=(QMetaObject::Connection connection);

public:
    void disconnect();

private:
    QList<QMetaObject::Connection> m_connections;
};