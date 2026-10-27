#include "LinkedInPageCollector.h"

#include <Utils.hpp>


QList<Vacancy> LinkedInPageCollector::parseSummaries(const QString& html) const
{
    static size_t id = 0;
    ++id;

    saveToFile(html, "Summary_" + QString::number(id) + ".html", "LinkedInPageCollector_Summaries");

    return LinkedInParserStrategy::parseSummaries(html);
}

Vacancy LinkedInPageCollector::parseDetails(const QString& html, const QUrl& url) const
{
    static size_t id = 0;
    ++id;

    saveToFile(html, "Details_" + QString::number(id) + ".html", "LinkedInPageCollector_Details");

    return LinkedInParserStrategy::parseDetails(html, url);
}
