// std

// Qt
#include <QThread>

// custom
#include "Vacancy.h"

namespace Linked
{
    Q_LOGGING_CATEGORY(LinkedVacancy, "vacancy");

    Vacancy::Vacancy()
    {
        clear();
    }

    qsizetype Vacancy::count(const QString &str, Qt::CaseSensitivity cs) const
    {
        return title.count(str, cs) + description.count(str, cs);
    }

    inline static constexpr const char* defaultStringValue = "NOT GIVEN";
    inline static constexpr Months defaultMonthsValue = 0;

    inline static const QUrl defaultUrlValue = QUrl();
    inline static const QDate defaultDateValue = QDate();

    void Vacancy::clear()
    {
        title = defaultStringValue;
        link = defaultUrlValue;

        companyName = defaultStringValue;
        industry = defaultStringValue;
        companyLink = defaultUrlValue;

        location = defaultStringValue;
        date = defaultDateValue;
        expirationDate = defaultDateValue;

        experience = defaultMonthsValue;
        education = defaultStringValue;
        skills = defaultStringValue;

        level = defaultStringValue;
        employmentType = defaultStringValue;

        description = defaultStringValue;
    }

    void Vacancy::mergeFrom(const Vacancy& vacancy)
    {
        if (title == defaultStringValue)
            title = vacancy.title;

        if (link == defaultUrlValue)
            link = vacancy.link;

        if (companyName == defaultStringValue)
            companyName = vacancy.companyName;

        if (industry == defaultStringValue)
            industry = vacancy.industry;

        if (companyLink == defaultUrlValue)
            companyLink = vacancy.companyLink;

        if (location == defaultStringValue)
            location = vacancy.location;

        if (date == defaultDateValue)
            date = vacancy.date;

        if (expirationDate == defaultDateValue)
            expirationDate = vacancy.expirationDate;

        if (experience == defaultMonthsValue)
            experience = vacancy.experience;

        if (education == defaultStringValue)
            education = vacancy.education;

        if (skills == defaultStringValue)
            skills = vacancy.skills;

        if (level == defaultStringValue)
            level = vacancy.level;

        if (employmentType == defaultStringValue)
            employmentType = vacancy.employmentType;

        if (description == defaultStringValue)
            description = vacancy.description;
    }

    QDebug operator<<(QDebug debug, const Vacancy& vacancy)
    {
        QDebugStateSaver saver(debug);

        debug.nospace()
            << "Vacancy(" << "\n"
            << "title:         " << vacancy.title << "\n"
            << "link:          " << vacancy.link << "\n"
            << "companyName:   " << vacancy.companyName << "\n"
            << "industry:      " << vacancy.industry << "\n"
            << "companyLink:   " << vacancy.companyLink << "\n"
            << "location:      " << vacancy.location << "\n"
            << "date:          " << vacancy.date << "\n"
            << "expirationDate:" << vacancy.expirationDate << "\n"
            << "experience:    " << vacancy.experience << "\n"
            << "level:         " << vacancy.level << "\n"
            << "education:     " << vacancy.education << "\n"
            << "skills:        " << vacancy.skills << "\n"
            << "employmentType:" << vacancy.employmentType << "\n"
            << "description:   " << vacancy.description << "\n"
            << ')';

        return debug;
    }

} // namespace Linked