#pragma once

#include <optional>

#include <QDebug>

template<typename T>
void print(QDebug& debug, const char* name, const T& value)
{
    debug << qSetFieldWidth(16) << name << qSetFieldWidth(0) << ": " << value << '\n';
}

template<typename T>
void print(QDebug& debug, const char* name, const std::optional<T>& opt)
{
    if (opt)
        print(debug, name, opt.value());
}