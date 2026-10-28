#pragma once

#include <ostream>

#include <QString>

inline void PrintTo(const QString& string, std::ostream* os)
{
    *os << string.toStdString();
}
