#include "LinkedInParserStrategy.h"

#include <QJsonDocument>
#include <QRegularExpression>

#include "LinkedInBriefVacancyExtractor.h"
#include "LinkedInDetailsHtmlVacancyExtractor.h"
#include "LinkedInDetailsJsonVacancyExtractor.h"
#include <Utils.hpp>


PageType LinkedInParserStrategy::classifyPage(const QString& html) const
{
    if (html.contains("description"))
        return PageType::Details;

    return PageType::Brief;
}

QList<Vacancy> LinkedInParserStrategy::parseBriefs(const QString& html) const
{
    const QStringList briefs = extractVacancyBriefs(html);

    LinkedInBriefVacancyExtractor extractor;

    QList<Vacancy> vacancies;
    vacancies.reserve(briefs.size());

    for (const QString& brief : briefs)
    {
        extractor.setHtml(brief);

        vacancies.push_back(extractor.vacancy());
    }

    return vacancies;
}

Vacancy LinkedInParserStrategy::parseDetails(const QString& html, const QUrl& url) const
{
    const QJsonObject json = extractJsonObject(html);
    if (!json.isEmpty())
    {
        const LinkedInDetailsJsonVacancyExtractor extractor(json, url);
        return extractor.vacancy();
    }

    const LinkedInDetailsHtmlVacancyExtractor extractor(html, url);
    return extractor.vacancy();
}
