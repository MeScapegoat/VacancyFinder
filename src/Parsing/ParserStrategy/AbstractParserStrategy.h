#pragma once

#include <QList>

#include <Vacancy.h>


enum class PageType
{
    Unknown,
    Brief,
    Details,


    COUNT
};


class AbstractParserStrategy
{
public:
    AbstractParserStrategy() = default;
    virtual ~AbstractParserStrategy() = default;

public:
    virtual PageType classifyPage(const QString& html) const = 0;

    virtual QList<Vacancy> parseBriefs(const QString& html) const = 0;
    virtual Vacancy parseDetails(const QString& html, const QUrl& url) const = 0;
};