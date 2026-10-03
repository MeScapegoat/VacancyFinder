#include "LinkedInDetailsJsonVacancyExtractor.h"

#include <Utils.hpp>


LinkedInDetailsJsonVacancyExtractor::LinkedInDetailsJsonVacancyExtractor(const QJsonObject& json_, const QUrl& url_) :
    json(json_),
    url(url_)
{
    static constexpr auto organizationKey = "hiringOrganization";
    const QJsonValue organizationValue = json.value(organizationKey);
    if (organizationValue.isObject())
    {
        organization = organizationValue.toObject();
    }
}

QString LinkedInDetailsJsonVacancyExtractor::title() const
{
    static constexpr auto titleFieldName = "title";

    const QJsonValue title = json.value(titleFieldName);
    if (title.isString())
    {
        return title.toString();
    }

    return VacancyExtractor::title();
};

QUrl LinkedInDetailsJsonVacancyExtractor::link() const
{
    return url;
};

QString LinkedInDetailsJsonVacancyExtractor::companyName() const
{
    static constexpr auto organizationNameKey = "name";

    if (organization.has_value())
    {
        const QJsonValue companyName = organization->value(organizationNameKey);
        if (companyName.isString())
        {
            return companyName.toString();
        }
    }

    return VacancyExtractor::companyName();
};

QUrl LinkedInDetailsJsonVacancyExtractor::companyLink() const
{
    static constexpr auto organizationUrlKey = "sameAs";

    if (organization.has_value())
    {
        const QJsonValue companyUrl = organization->value(organizationUrlKey);
        if (companyUrl.isString())
        {
            return companyUrl.toString();
        }
    }

    return VacancyExtractor::companyLink();
};

QString LinkedInDetailsJsonVacancyExtractor::location() const
{
    static constexpr auto jobLocationKey = "jobLocation";
    static constexpr auto addressKey = "address";
    static constexpr auto addressLocalityKey = "addressLocality";

    const QJsonValue jobLocation = json.value(jobLocationKey);
    if (jobLocation.isObject())
    {
        const QJsonValue address = jobLocation.toObject().value(addressKey);
        if (address.isObject())
        {
            const QJsonValue locality = address.toObject().value(addressLocalityKey);
            if (locality.isString())
            {
                return locality.toString();
            }
        }
    }

    return VacancyExtractor::location();
};

QString LinkedInDetailsJsonVacancyExtractor::employmentType() const
{
    static constexpr auto employmentTypeKey = "employmentType";

    const QJsonValue employmentType = json.value(employmentTypeKey);
    if (employmentType.isString())
    {
        return employmentType.toString();
    }

    return VacancyExtractor::employmentType();
};

QString LinkedInDetailsJsonVacancyExtractor::skills() const
{
    static constexpr auto skillsKey = "skills";

    const QJsonValue skills = json.value(skillsKey);
    if (skills.isString())
    {
        return skills.toString();
    }

    return VacancyExtractor::skills();
};

QString LinkedInDetailsJsonVacancyExtractor::description() const
{
    static constexpr auto descriptionKey = "description";

    const QJsonValue description = json.value(descriptionKey);
    if (description.isString())
    {
        return decodeHtml(description.toString());
    }

    return VacancyExtractor::description();
};

QDate LinkedInDetailsJsonVacancyExtractor::date() const
{
    static constexpr auto dateKey = "datePosted";

    const QJsonValue date = json.value(dateKey);
    if (date.isString())
    {
        return QDate::fromString(date.toString(), Qt::ISODate);
    }

    return VacancyExtractor::date();
};

std::optional<QString> LinkedInDetailsJsonVacancyExtractor::industry() const
{
    static constexpr auto industryKey = "industry";

    const QJsonValue industry = json.value(industryKey);
    if (industry.isString())
    {
        return industry.toString();
    }

    return VacancyExtractor::industry();
};

std::optional<Months> LinkedInDetailsJsonVacancyExtractor::experience() const
{
    static constexpr auto experienceRequirementsKey = "experienceRequirements";
    static constexpr auto monthsOfExperienceKey = "monthsOfExperience";

    const QJsonValue experienceRequirements = json.value(experienceRequirementsKey);
    if (experienceRequirements.isObject())
    {
        const QJsonValue monthsOfExperience = experienceRequirements.toObject().value(monthsOfExperienceKey);
        if (monthsOfExperience.isDouble())
        {
            return monthsOfExperience.toInt();
        }
    }

    return VacancyExtractor::experience();
};

std::optional<QString> LinkedInDetailsJsonVacancyExtractor::education() const
{
    static constexpr auto educationRequirementsKey = "educationRequirements";
    static constexpr auto credentialCategoryKey = "credentialCategory";

    const QJsonValue educationRequirements = json.value(educationRequirementsKey);
    if (educationRequirements.isObject())
    {
        const QJsonValue credentialCategory = educationRequirements.toObject().value(credentialCategoryKey);
        if (credentialCategory.isString())
        {
            return credentialCategory.toString();
        }
    }

    return VacancyExtractor::education();
};

std::optional<QDate> LinkedInDetailsJsonVacancyExtractor::expirationDate() const
{
    static constexpr auto expirationDateKey = "validThrough";

    const QJsonValue expirationDate = json.value(expirationDateKey);
    if (expirationDate.isString())
    {
        return QDate::fromString(expirationDate.toString(), Qt::ISODate);
    }

    return VacancyExtractor::date();
};

QJsonObject extractJsonObject(const QString& html)
{
    static constexpr auto jsonObjectPattern = R"REGEX(<script\b[^>]*type="application/ld\+json"[^>]*>(.*?)</script>)REGEX";

    const QString jsonText = extract(html, jsonObjectPattern);
    const QJsonDocument document = QJsonDocument::fromJson(jsonText.toUtf8());

    return document.object();
}