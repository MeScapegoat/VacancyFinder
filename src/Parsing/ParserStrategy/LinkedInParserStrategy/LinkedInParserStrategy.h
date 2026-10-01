#pragma once

#include <AbstractParserStrategy.h>
#include <QLoggingCategory>
#include <QJsonObject>

#include <Vacancy.h>


Q_DECLARE_LOGGING_CATEGORY(LinkedInParserStrategyLog);


class LinkedInParserStrategy : public AbstractParserStrategy
{
public:
    LinkedInParserStrategy() = default;
    virtual ~LinkedInParserStrategy() = default;

public: // AbstractParserStrategy
    virtual PageType classifyPage(const QString& html) const override;

    virtual QList<Vacancy> parseBriefs(const QString& html) const override;
    virtual Vacancy parseDetails(const QString& html, const QUrl& url) const override;
};


Vacancy parseBrief(const QString& html);

Vacancy parseDetailsByJson(const QJsonObject& object, const QUrl& url);
Vacancy parseDetailsByHtml(const QString& html, const QUrl& url);

QJsonObject extractJsonObject(const QString& html);