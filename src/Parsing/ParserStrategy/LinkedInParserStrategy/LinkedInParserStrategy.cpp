#include "LinkedInParserStrategy.h"

#include <QRegularExpression>
#include <QJsonDocument>
#include <QDebug>

#include <Utils.hpp>


Q_LOGGING_CATEGORY(LinkedInParserStrategyLog, "parsing.linkedInParserStrategy");


PageType LinkedInParserStrategy::classifyPage(const QString& html) const
{
    if (html.contains("description"))
        return PageType::Details;

    return PageType::Brief;
}

QList<Vacancy> LinkedInParserStrategy::parseBriefs(const QString& html) const
{
    static const QRegularExpression vacancyRegex(R"(<li\b[^>]*>(.*?)</li>)", QRegularExpression::DotMatchesEverythingOption);


    const QString cleanHtml = removeSpaces(html);

    QRegularExpressionMatchIterator it = vacancyRegex.globalMatch(cleanHtml);
    QStringList data;

    while (it.hasNext())
    {
        const QRegularExpressionMatch match = it.next();

        data.emplaceBack(match.captured(1));
    }


    QList<Vacancy> vacancies;
    vacancies.reserve(data.size());

    for (const QString& html : data)
    {
        vacancies.push_back(parseBrief(html));
    }

    return vacancies;
}

Vacancy LinkedInParserStrategy::parseDetails(const QString& html, const QUrl& url) const
{
    const QString cleanHtml = removeSpaces(html);
    const QJsonObject json = extractJsonObject(cleanHtml);

    qCDebug(LinkedInParserStrategyLog) << json;

    Vacancy vacancy;

    if (!json.isEmpty())
    {
        vacancy = parseDetailsByJson(json, url);
    }
    else
    {
        vacancy = parseDetailsByHtml(cleanHtml, url);
    }

    return vacancy;
}

Vacancy parseBrief(const QString& html)
{
    static constexpr const char* titlePattern = R"(<h3\b[^>]*class="base-search-card__title"[^>]*>(.*?)</h3>)";
    static constexpr const char* linkPattern = R"REGEX(<a\b[^>]*class="base-card__full-link[^"]*"[^>]*href="([^"]+)")REGEX";
    static constexpr const char* alternateLinkPattern = R"REGEX(<a\s+href="(https://rs\.linkedin\.com/jobs/view/[^"]+)")REGEX";

    static constexpr const char* companyNamePattern = R"(<h4\b[^>]*class="base-search-card__subtitle"[^>]*>.*?<a\b[^>]*>(.*?)</a>)";
    static constexpr const char* companyLinkPattern = R"REGEX(<h4\b[^>]*class="base-search-card__subtitle"[^>]*>.*?<a\b[^>]*href="([^"]+)")REGEX";

    static constexpr const char* locationPattern = R"(<span\b[^>]*class="job-search-card__location"[^>]*>(.*?)</span>)";
    static constexpr const char* datePattern = R"REGEX(<time\b[^>]*datetime="([^"]+)")REGEX";


    //static constexpr const char* dateFormat = "dd-MM-yyyy";


    Vacancy vacancy;

    vacancy.title = extract(html, titlePattern);

    vacancy.link = extract(html, linkPattern);

    if (!vacancy.link.isValid())
    {
        vacancy.link = extract(html, alternateLinkPattern);
    }
    vacancy.link.setQuery(QString());

    vacancy.companyName = extract(html, companyNamePattern);

    QUrl companyLink = extract(html, companyLinkPattern);
    companyLink.setQuery(QString());

    vacancy.companyLink = companyLink;

    vacancy.location = extract(html, locationPattern);
    
    // employmentType;
    // skills;
    // description;

    const QString date = extract(html, datePattern);
    vacancy.date = QDate::fromString(date, Qt::ISODate);

    // industry;
    // level;
    // experience;
    // education;
    // expirationDate;

    return vacancy;
}

Vacancy parseDetailsByJson(const QJsonObject& object, const QUrl& url)
{
    static constexpr const char* titleFieldName = "title";

    static constexpr const char* organizationFieldName = "hiringOrganization";
    static constexpr const char* organizationNameFieldName = "name";
    static constexpr const char* organizationLinkFieldName = "sameAs";

    static constexpr const char* industryFieldName = "industry";

    static constexpr const char* locationFieldName = "jobLocation";
    static constexpr const char* locationAddressFieldName = "address";
    static constexpr const char* locationAddressLocalityFieldName = "addressLocality";

    static constexpr const char* dateFieldName = "datePosted";
    static constexpr const char* expirationDateFieldName = "validThrough";

    static constexpr const char* experienceFieldName = "experienceRequirements";
    static constexpr const char* experienceMonthsFieldName = "monthsOfExperience";

    static constexpr const char* educationFieldName = "educationRequirements";
    static constexpr const char* educationCategoryFieldName = "credentialCategory";

    static constexpr const char* skillsFieldName = "skills";

    static constexpr const char* employmentTypeFieldName = "employmentType";

    static constexpr const char* descriptionTypeFieldName = "description";

    qCDebug(LinkedInParserStrategyLog) << object;

    Vacancy vacancy;

    vacancy.title = object[titleFieldName].toString();
    vacancy.link = url;

    vacancy.companyName = object[organizationFieldName][organizationNameFieldName].toString();
    vacancy.companyLink = object[organizationFieldName][organizationLinkFieldName].toString();

    vacancy.location = object[locationFieldName][locationAddressFieldName][locationAddressLocalityFieldName].toString();

    vacancy.employmentType = object[employmentTypeFieldName].toString();
    vacancy.skills = object[skillsFieldName].toString();

    vacancy.description = object[descriptionTypeFieldName].toString();

    vacancy.date = QDate::fromString(object[dateFieldName].toString(), Qt::ISODate);

    vacancy.industry = object[industryFieldName].toString();
    //vacancy.level;
    vacancy.experience = object[experienceFieldName][experienceMonthsFieldName].toInt();
    vacancy.education = object[educationFieldName][educationCategoryFieldName].toString();
    vacancy.expirationDate = QDate::fromString(object[expirationDateFieldName].toString(), Qt::ISODate);


    return vacancy;
}

Vacancy parseDetailsByHtml(const QString& html, const QUrl& url)
{
    static constexpr const char* titlePattern = R"REGEX(<h1\b[^>]*class="top-card-layout__title[^"]*"[^>]*>(.*?)</h1>)REGEX";
    //static constexpr const char* linkPattern = R"REGEX(<link\b[^>]*rel="canonical"[^>]*href="([^"]+)")REGEX";

    static constexpr const char* companyNamePattern = R"REGEX(<a\b[^>]*class="topcard__org-name-link[^"]*"[^>]*>(.*?)</a>)REGEX";
    static constexpr const char* companyLinkPattern = R"REGEX(<a\b[^>]*class="topcard__org-name-link[^"]*"[^>]*href="([^"]+)")REGEX";

    static constexpr const char* locationPattern = R"REGEX(<span\b[^>]*class="topcard__flavor[^"]*topcard__flavor--bullet[^"]*"[^>]*>(.*?)</span>)REGEX";

    static constexpr const char* descriptionPattern = R"REGEX(<div\b[^>]*class="show-more-less-html__markup[^"]*"[^>]*>(.*?)</div>)REGEX";

    static constexpr const char* jobCriteriaPattern = R"REGEX(<span\b[^>]*class="description__job-criteria-text[^"]*description__job-criteria-text--criteria[^"]*"[^>]*>(.*?)</span>)REGEX";

    //static constexpr const char* сlosedJobStr = "closed-job";

    const QStringList jobCriteriaList = extractAll(html, jobCriteriaPattern);

    Vacancy vacancy;

    vacancy.title = extract(html, titlePattern);
    vacancy.link = url;

    vacancy.companyName = extract(html, companyNamePattern);
    vacancy.companyLink = extract(html, companyLinkPattern);
    vacancy.companyLink.setQuery(QString());

    vacancy.location = extract(html, locationPattern);

    if (jobCriteriaList.size() > 1) vacancy.employmentType = jobCriteriaList[1];
    // skills;
    vacancy.description = extract(html, descriptionPattern);
    // date;
    if (jobCriteriaList.size() > 3) vacancy.industry = jobCriteriaList[3];
    if (jobCriteriaList.size() > 0) vacancy.level = jobCriteriaList[0];
    // experience;
    // education;
    // expirationDate;

    return vacancy;
}

QJsonObject extractJsonObject(const QString& html)
{
    static constexpr auto jsonObjectPattern = R"REGEX(<script\b[^>]*type="application/ld\+json"[^>]*>(.*?)</script>)REGEX";

    const QString jsonText = extract(html, jsonObjectPattern);
    const QJsonDocument document = QJsonDocument::fromJson(jsonText.toUtf8());

    return document.object();
}
