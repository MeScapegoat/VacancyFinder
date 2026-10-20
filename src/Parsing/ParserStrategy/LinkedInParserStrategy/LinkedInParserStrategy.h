#pragma once

#include <AbstractParserStrategy.h>
#include <QLoggingCategory>
#include <QJsonObject>

#include <Vacancy.h>


Q_DECLARE_LOGGING_CATEGORY(LinkedInParserStrategyLog);


class LinkedInParserStrategy : public AbstractParserStrategy
{
public:
    explicit LinkedInParserStrategy(QObject* parent = nullptr) :
        AbstractParserStrategy(parent)
    {}
    virtual ~LinkedInParserStrategy() = default;

public: // AbstractParserStrategy
    virtual PageType classifyPage(const QString& html) const override;

    virtual QList<Vacancy> parseBriefs(const QString& html) const override;
    virtual Vacancy parseDetails(const QString& html, const QUrl& url) const override;
};