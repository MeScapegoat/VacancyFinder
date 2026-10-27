#pragma once

#include <QJsonObject>

#include <AbstractParserStrategy.h>
#include <Vacancy.h>


class LinkedInParserStrategy : public AbstractParserStrategy
{
public:
    explicit LinkedInParserStrategy(QObject* parent = nullptr) :
        AbstractParserStrategy(parent)
    {}
    virtual ~LinkedInParserStrategy() = default;

public: // AbstractParserStrategy
    virtual PageType classifyPage(const QString& html) const override;

    virtual QList<Vacancy> parseSummaries(const QString& html) const override;
    virtual Vacancy parseDetails(const QString& html, const QUrl& url) const override;
};
