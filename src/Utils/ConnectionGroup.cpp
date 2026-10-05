#include "ConnectionGroup.h"

#include <QObject>

ConnectionGroup::~ConnectionGroup()
{
    disconnect();
}

ConnectionGroup& ConnectionGroup::operator+=(QMetaObject::Connection connection)
{
    m_connections.append(connection);
    return *this;
}

void ConnectionGroup::disconnect()
{
    for (QMetaObject::Connection& connection : m_connections)
    {
        QObject::disconnect(connection);
    }

    m_connections.clear();
}
