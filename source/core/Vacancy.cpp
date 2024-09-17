// std

// Qt
#include <QThread>

// custom
#include "Vacancy.h"

namespace Linked
{
    Q_LOGGING_CATEGORY(LinkedVacancy, "vacancy");

    void Vacancy::print(QTextStream &stream) const
    {
        stream << "           title | " << title << "\n";
        stream << "            link | " << link << "\n";
        stream << "     companyName | " << companyName << "\n";
        stream << "     companyLink | " << companyLink << "\n";
        stream << "           level | " << level << "\n";
        stream << "employement type | " << employmentType << "\n";
        stream << "        location | " << location << "\n";
        stream << "            date | " << date.toString("dd-MM-yyyy") << "\n";
        stream << "     description | " << description << "\n";
        stream << QString(200, '-') << "\n";
    }

    qsizetype Vacancy::count(const QString &str, Qt::CaseSensitivity cs) const
    {
        return title.count(str, cs) + description.count(str, cs);
    }

    void Vacancy::clear()
    {
        title = "Not Given";
        link = "Not Given";
        companyName = "Not Given";
        companyLink = "Not Given";
        description = "Not Given";
        location = "Not Given";
        level = "Not Given";
        employmentType = "Not Given";

        date = QDate();
    }

} // namespace Linked