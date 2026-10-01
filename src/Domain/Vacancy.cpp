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

void Vacancy::fillEmptyFrom(Vacancy other) noexcept
{
    // cringe alarm

    if (title.isEmpty())             title = std::move(other.title);
    if (link.isEmpty())              link = std::move(other.link);
    if (companyName.isEmpty())       companyName = std::move(other.companyName);
    if (companyLink.isEmpty())       companyLink = std::move(other.companyLink);
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
    print(debug, "link", vacancy.link);
    print(debug, "companyName", vacancy.companyName);
    print(debug, "companyLink", vacancy.companyLink);
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