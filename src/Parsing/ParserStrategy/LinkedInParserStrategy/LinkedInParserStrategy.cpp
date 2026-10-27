#include "LinkedInParserStrategy.h"

#include <QJsonDocument>
#include <QRegularExpression>

#include "LinkedInSummaryVacancyExtractor.h"
#include "LinkedInDetailsHtmlVacancyExtractor.h"
#include "LinkedInDetailsJsonVacancyExtractor.h"
#include <Utils.hpp>


PageType LinkedInParserStrategy::classifyPage(const QString& html) const
{
    if (html.contains("description"))
        return PageType::Details;

    return PageType::Summary;
}

QList<Vacancy> LinkedInParserStrategy::parseSummaries(const QString& html) const
{
    const QStringList summaries = extractVacancySummaries(html);

    LinkedInSummaryVacancyExtractor extractor;

    QList<Vacancy> vacancies;
    vacancies.reserve(summaries.size());

    for (const QString& summary : summaries)
    {
        extractor.setHtml(summary);

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
