#include "LinkedInDetailsHtmlVacancyExtractor.h"

#include <Utils.hpp>


LinkedInDetailsHtmlVacancyExtractor::LinkedInDetailsHtmlVacancyExtractor(const QString& html_, const QUrl& url_) :
    html(html_),
    vacancyUrl(url_)
{
    static constexpr auto jobCriteriaPattern = R"REGEX(<span\b[^>]*class="description__job-criteria-text[^"]*description__job-criteria-text--criteria[^"]*"[^>]*>(.*?)</span>)REGEX";

    jobCriteriaList = extractAll(html, jobCriteriaPattern);
}

QString LinkedInDetailsHtmlVacancyExtractor::title() const
{
    static constexpr auto titlePattern = R"REGEX(<h1\b[^>]*class="top-card-layout__title[^"]*"[^>]*>(.*?)</h1>)REGEX";

    return extract(html, titlePattern);
};

QUrl LinkedInDetailsHtmlVacancyExtractor::url() const
{
    return vacancyUrl;
};

QString LinkedInDetailsHtmlVacancyExtractor::companyName() const
{
    static constexpr auto companyNamePattern = R"REGEX(<a\b[^>]*class="topcard__org-name-link[^"]*"[^>]*>(.*?)</a>)REGEX";

    return extract(html, companyNamePattern);
};

QUrl LinkedInDetailsHtmlVacancyExtractor::companyUrl() const
{
    static constexpr auto companyUrlPattern = R"REGEX(<a\b[^>]*class="topcard__org-name-link[^"]*"[^>]*href="([^"]+)")REGEX";

    QUrl companyUrl = extract(html, companyUrlPattern);
    companyUrl.setQuery(QString());

    return companyUrl;
};

QString LinkedInDetailsHtmlVacancyExtractor::location() const
{
    static constexpr auto locationPattern = R"REGEX(<span\b[^>]*class="topcard__flavor[^"]*topcard__flavor--bullet[^"]*"[^>]*>(.*?)</span>)REGEX";

    return extract(html, locationPattern);
};

QString LinkedInDetailsHtmlVacancyExtractor::employmentType() const
{
    if (jobCriteriaList.size() > 1)
    {
        return jobCriteriaList[1];
    }

    return VacancyExtractor::employmentType();
};

QString LinkedInDetailsHtmlVacancyExtractor::description() const
{
    static constexpr auto descriptionPattern = R"REGEX(<div\b[^>]*class="show-more-less-html__markup[^"]*"[^>]*>(.*?)</div>)REGEX";

    return extract(html, descriptionPattern);
};

std::optional<QString> LinkedInDetailsHtmlVacancyExtractor::industry() const
{
    if (jobCriteriaList.size() > 3)
    {
        return jobCriteriaList[3];
    }

    return VacancyExtractor::industry();
};

std::optional<QString> LinkedInDetailsHtmlVacancyExtractor::level() const
{
    if (jobCriteriaList.size() > 0)
    {
        return jobCriteriaList[0];
    }

    return VacancyExtractor::level();
};