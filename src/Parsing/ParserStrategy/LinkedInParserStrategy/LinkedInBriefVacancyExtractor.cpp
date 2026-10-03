#include "LinkedInBriefVacancyExtractor.h"

#include <Utils.hpp>


void LinkedInBriefVacancyExtractor::setHtml(const QString& html_)
{
    html = html_;
}

QString LinkedInBriefVacancyExtractor::title() const
{
    static constexpr auto titlePattern = R"(<h3\b[^>]*class="base-search-card__title"[^>]*>(.*?)</h3>)";

    return extract(html, titlePattern);
};

QUrl LinkedInBriefVacancyExtractor::link() const
{
    static constexpr auto linkPattern = R"REGEX(<a\b[^>]*class="base-card__full-link[^"]*"[^>]*href="([^"]+)")REGEX";
    static constexpr auto alternateLinkPattern = R"REGEX(<a\s+href="(https://rs\.linkedin\.com/jobs/view/[^"]+)")REGEX";

    QUrl link;

    link = extract(html, linkPattern);

    if (!link.isValid())
    {
        link = extract(html, alternateLinkPattern);
    }
    link.setQuery(QString());

    return link;
};

QString LinkedInBriefVacancyExtractor::companyName() const
{
    static constexpr auto companyNamePattern = R"(<h4\b[^>]*class="base-search-card__subtitle"[^>]*>.*?<a\b[^>]*>(.*?)</a>)";

    return extract(html, companyNamePattern);
};

QUrl LinkedInBriefVacancyExtractor::companyLink() const
{
    static constexpr auto companyLinkPattern = R"REGEX(<h4\b[^>]*class="base-search-card__subtitle"[^>]*>.*?<a\b[^>]*href="([^"]+)")REGEX";

    QUrl companyLink = extract(html, companyLinkPattern);
    companyLink.setQuery(QString());

    return companyLink;
};

QString LinkedInBriefVacancyExtractor::location() const
{
    static constexpr auto locationPattern = R"(<span\b[^>]*class="job-search-card__location"[^>]*>(.*?)</span>)";

    return extract(html, locationPattern);
};

QDate LinkedInBriefVacancyExtractor::date() const
{
    static constexpr auto datePattern = R"REGEX(<time\b[^>]*datetime="([^"]+)")REGEX";

    const QString date = extract(html, datePattern);
    return QDate::fromString(date, Qt::ISODate);
};

QStringList extractVacancyBriefs(const QString& html)
{
    static const QRegularExpression vacancyBriefRegex(R"(<li\b[^>]*>(.*?)</li>)", QRegularExpression::DotMatchesEverythingOption);

    QRegularExpressionMatchIterator it = vacancyBriefRegex.globalMatch(html);
    QStringList briefs;

    while (it.hasNext())
    {
        const QRegularExpressionMatch match = it.next();
        briefs.emplaceBack(match.captured(1));
    }

    return briefs;
}