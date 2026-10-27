#pragma once

#include <QList>
#include <QObject>

#include <Vacancy.h>


enum class PageType
{
    Unknown,
    Summary,
    Details,


    COUNT
};


class AbstractParserStrategy : public QObject
{
public:
    explicit AbstractParserStrategy(QObject* parent = nullptr) : 
        QObject(parent)
    {}
    virtual ~AbstractParserStrategy() = default;

public:
    virtual PageType classifyPage(const QString& html) const = 0;

    virtual QList<Vacancy> parseSummaries(const QString& html) const = 0;
    virtual Vacancy parseDetails(const QString& html, const QUrl& url) const = 0;
};
