#pragma once

#include <ostream>

#include <QString>

void PrintTo(const QString& string, std::ostream* os)
{
    *os << string.toStdString();
}