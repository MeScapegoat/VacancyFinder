#include "LinkedInPageCollector.h"

#include <Utils.hpp>


QList<Vacancy> LinkedInPageCollector::parseBriefs(const QString& html) const
{
    static size_t id = 0;
    ++id;

    saveToFile(html, "Brief_" + QString::number(id) + ".html", "LinkedInPageCollector_Briefs");

    return LinkedInParserStrategy::parseBriefs(html);
}

Vacancy LinkedInPageCollector::parseDetails(const QString& html, const QUrl& url) const
{
    static size_t id = 0;
    ++id;

    saveToFile(html, "Details_" + QString::number(id) + ".html", "LinkedInPageCollector_Details");

    return LinkedInParserStrategy::parseDetails(html, url);
}
