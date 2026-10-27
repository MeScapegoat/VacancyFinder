#include "LinkedInSummaryVacancyExtractor.h"

#include <Utils.hpp>


void LinkedInSummaryVacancyExtractor::setHtml(const QString& html_)
{
    html = html_;
}

QString LinkedInSummaryVacancyExtractor::title() const
{
    static constexpr auto titlePattern = R"(<h3\b[^>]*class="base-search-card__title"[^>]*>(.*?)</h3>)";

    return extract(html, titlePattern);
};

QUrl LinkedInSummaryVacancyExtractor::url() const
{
    static constexpr auto urlPattern = R"REGEX(<a\b[^>]*class="base-card__full-link[^"]*"[^>]*href="([^"]+)")REGEX";
    static constexpr auto alternateUrlPattern = R"REGEX(<a\s+href="(https://rs\.linkedin\.com/jobs/view/[^"]+)")REGEX";

    QUrl url;

    url = extract(html, urlPattern);

    if (!url.isValid())
    {
        url = extract(html, alternateUrlPattern);
    }
    url.setQuery(QString());

    return url;
};

QString LinkedInSummaryVacancyExtractor::companyName() const
{
    static constexpr auto companyNamePattern = R"(<h4\b[^>]*class="base-search-card__subtitle"[^>]*>.*?<a\b[^>]*>(.*?)</a>)";

    return extract(html, companyNamePattern);
};

QUrl LinkedInSummaryVacancyExtractor::companyUrl() const
{
    static constexpr auto companyUrlPattern = R"REGEX(<h4\b[^>]*class="base-search-card__subtitle"[^>]*>.*?<a\b[^>]*href="([^"]+)")REGEX";

    QUrl companyUrl = extract(html, companyUrlPattern);
    companyUrl.setQuery(QString());

    return companyUrl;
};

QString LinkedInSummaryVacancyExtractor::location() const
{
    static constexpr auto locationPattern = R"(<span\b[^>]*class="job-search-card__location"[^>]*>(.*?)</span>)";

    return extract(html, locationPattern);
};

QDate LinkedInSummaryVacancyExtractor::date() const
{
    static constexpr auto datePattern = R"REGEX(<time\b[^>]*datetime="([^"]+)")REGEX";

    const QString date = extract(html, datePattern);
    return QDate::fromString(date, Qt::ISODate);
};

QStringList extractVacancySummaries(const QString& html)
{
    static const QRegularExpression vacancySummaryRegex(R"(<li\b[^>]*>(.*?)</li>)", QRegularExpression::DotMatchesEverythingOption);

    QRegularExpressionMatchIterator it = vacancySummaryRegex.globalMatch(html);
    QStringList summaries;

    while (it.hasNext())
    {
        const QRegularExpressionMatch match = it.next();
        summaries.emplaceBack(match.captured(1));
    }

    return summaries;
}