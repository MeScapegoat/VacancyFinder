#include "Vacancy.h"

#include <Utils.hpp>


Q_LOGGING_CATEGORY(VacancyLog, "vacancy");


qsizetype Vacancy::count(const QString& str, Qt::CaseSensitivity cs) const noexcept
{
    qsizetype counted = 0;

    counted += title.count(str, cs);

    counted += description.count(str, cs);

    return counted;
}

bool Vacancy::contains(const QString& str, Qt::CaseSensitivity cs) const noexcept
{
    return title.contains(str, cs) || description.contains(str, cs);
}

void Vacancy::fillEmptyFrom(Vacancy other) noexcept
{
    // cringe alarm

    if (title.isEmpty())             title = std::move(other.title);
    if (url.isEmpty())               url = std::move(other.url);
    if (companyName.isEmpty())       companyName = std::move(other.companyName);
    if (companyUrl.isEmpty())        companyUrl = std::move(other.companyUrl);
    if (location.isEmpty())          location = std::move(other.location);
    if (employmentType.isEmpty())    employmentType = std::move(other.employmentType);
    if (skills.isEmpty())            skills = std::move(other.skills);
    if (description.isEmpty())       description = std::move(other.description);
    if (!date.isValid())             date = std::move(other.date);
    if (!industry.has_value())       industry = std::move(other.industry);
    if (!level.has_value())          level = std::move(other.level);
    if (!experience.has_value())     experience = std::move(other.experience);
    if (!education.has_value())      education = std::move(other.education);
    if (!expirationDate.has_value()) expirationDate = std::move(other.expirationDate);
}

QDebug operator<<(QDebug debug, const Vacancy& vacancy) noexcept
{
    // cringe alarm

    QDebugStateSaver saver(debug);

    debug.nospace() << "Vacancy:\n";
    print(debug, "id", vacancy.id);
    print(debug, "title", vacancy.title);
    print(debug, "url", vacancy.url);
    print(debug, "companyName", vacancy.companyName);
    print(debug, "companyUrl", vacancy.companyUrl);
    print(debug, "location", vacancy.location);
    print(debug, "employmentType", vacancy.employmentType);
    print(debug, "skills", vacancy.skills);
    print(debug, "description", vacancy.description);
    print(debug, "date", vacancy.date);
    print(debug, "industry", vacancy.industry);
    print(debug, "level", vacancy.level);
    print(debug, "experience", vacancy.experience);
    print(debug, "education", vacancy.education);
    print(debug, "expirationDate", vacancy.expirationDate);

    return debug;
}