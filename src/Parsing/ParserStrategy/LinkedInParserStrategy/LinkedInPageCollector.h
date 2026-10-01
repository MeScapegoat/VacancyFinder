#pragma once

#include "LinkedInParserStrategy.h"


class LinkedInPageCollector : public LinkedInParserStrategy
{
public:
    LinkedInPageCollector() = default;
    virtual ~LinkedInPageCollector() = default;

public: // LinkedInParserStrategy
    virtual QList<Vacancy> parseBriefs(const QString& html) const override;
    virtual Vacancy parseDetails(const QString& html, const QUrl& url) const override;
};