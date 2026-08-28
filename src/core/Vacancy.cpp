#include "Vacancy.h"

Q_LOGGING_CATEGORY(VacancyLog, "vacancy");

Vacancy::Vacancy()
{
    qCInfo(VacancyLog) << __FUNCTION__;

    reset();
}

Vacancy::~Vacancy()
{
    qCInfo(VacancyLog) << __FUNCTION__;
}

qsizetype Vacancy::count(const QString& str, Qt::CaseSensitivity cs) const
{
    qsizetype counted = 0;

    if (title)
    {
        counted += title.value().count(str, cs);
    }

    if (description)
    {
        counted += description.value().count(str, cs);
    }

    return counted;
}

void Vacancy::reset()
{
    id = 0;

    link = QUrl();

    title.reset();

    companyName.reset();
    industry.reset();
    companyLink.reset();

    location.reset();
    date.reset();
    expirationDate.reset();

    experience.reset();
    education.reset();
    skills.reset();

    level.reset();
    employmentType.reset();

    description.reset();
}

void Vacancy::mergeFrom(const Vacancy& vacancy)
{
    if (link.isEmpty())
        link = vacancy.link;

    if (!title)
        title = vacancy.title;

    if (!companyName)
        companyName = vacancy.companyName;

    if (!industry)
        industry = vacancy.industry;

    if (!companyLink)
        companyLink = vacancy.companyLink;

    if (!location)
        location = vacancy.location;

    if (!date)
        date = vacancy.date;

    if (!expirationDate)
        expirationDate = vacancy.expirationDate;

    if (!experience)
        experience = vacancy.experience;

    if (!education)
        education = vacancy.education;

    if (!skills)
        skills = vacancy.skills;

    if (!level)
        level = vacancy.level;

    if (!employmentType)
        employmentType = vacancy.employmentType;

    if (!description)
        description = vacancy.description;
}

QDebug operator<<(QDebug debug, const Vacancy& vacancy)
{

    QDebugStateSaver saver(debug);

    debug.nospace() << "Vacancy:\n";

    printVacancyItem(debug, "id", vacancy.id);
    printVacancyItem(debug, "link", vacancy.link);
    printVacancyItem(debug, "title", vacancy.title);
    printVacancyItem(debug, "companyName", vacancy.companyName);
    printVacancyItem(debug, "industry", vacancy.industry);
    printVacancyItem(debug, "companyLink", vacancy.companyLink);
    printVacancyItem(debug, "location", vacancy.location);
    printVacancyItem(debug, "date", vacancy.date);
    printVacancyItem(debug, "expirationDate", vacancy.expirationDate);
    printVacancyItem(debug, "experience", vacancy.experience);
    printVacancyItem(debug, "level", vacancy.level);
    printVacancyItem(debug, "education", vacancy.education);
    printVacancyItem(debug, "skills", vacancy.skills);
    printVacancyItem(debug, "employmentType", vacancy.employmentType);
    printVacancyItem(debug, "description", vacancy.description);

    return debug;
}