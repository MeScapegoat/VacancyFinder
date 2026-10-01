#pragma once

#include <optional>

#include <QDebug>
#include <QString>
#include <QRegularExpression>

QString removeSpaces(QString str);

QString extract(const QString& html, const QString& pattern);
QStringList extractAll(const QString& html, const QString& pattern);

QString decodeHtml(QString text);

void saveToFile(const QString& str, const QString& fileName, const QString& folderName);


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