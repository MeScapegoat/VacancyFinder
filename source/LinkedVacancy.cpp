#include <iostream> //tmp

#include "LinkedVacancy.h"
#include "LinkedApp.h"
LinkedVacancy::LinkedVacancy(const QString &data) {}

void LinkedVacancy::loadTitle(QString data)
{
    QString linkStart = "href=\"", linkEnd = ";";
    QString titleStart = "class=\"base-search-card__title\">\n", titleEnd = "\n";
    QString companyLinkStart = "href=\"", companyLinkEnd = "\"";
    QString companyNameStart = ">\n", companyNameEnd = "\n";
    QString alter_companyNameStart = "class=\"base-search-card__subtitle\">\n", alter_companyNameEnd = "\n";
    QString locationStart = "class=\"job-search-card__location\">\n", locationEnd = "\n";
    QString publishTimeStart = "datetime=\"", publishTimeEnd = "\"";

    link = extract(data, linkStart, linkEnd);
    data = data.mid(link.size() + data.indexOf(link));

    title = extract(data, titleStart, titleEnd);
    data = data.mid(title.size() + data.indexOf(title));

    companyLink = extract(data, companyLinkStart, companyLinkEnd);
    data = data.mid(companyLink.size() + data.indexOf(companyLink));

    if (companyLink.contains("http"))
        companyName = extract(data, companyNameStart, companyNameEnd);
    else
        companyName = extract(data, alter_companyNameStart, alter_companyNameEnd);
    data = data.mid(companyName.size() + data.indexOf(companyName));

    location = extract(data, locationStart, locationEnd);
    data = data.mid(location.size() + data.indexOf(location));

    QString dateStr = extract(data, publishTimeStart, publishTimeEnd);
    date = QDate::fromString(dateStr, "yyyy-MM-dd");
    data = data.mid(dateStr.size() + data.indexOf(dateStr));
}

void LinkedVacancy::loadDescription(QString data)
{
    QString descriptionStart = "relative overflow-hidden\">\n", descriptionEnd = "\n";
    QString levelStart = "<span class=\"description__job-criteria-text description__job-criteria-text--criteria\">\n", levelEnd = "\n";
    QString employmentTypeStart = "<span class=\"description__job-criteria-text description__job-criteria-text--criteria\">\n", employmentTypeEnd = "\n";

    description = extract(data, descriptionStart, descriptionEnd);
    data = data.mid(description.size() + data.indexOf(description));

    if (data.contains("Seniority level"))
    {
        level = extract(data, levelStart, levelEnd);
        data = data.mid(level.size() + data.indexOf(level));
    }

    if (data.contains("Employment type"))
    {
        employmentType = extract(data, employmentTypeStart, employmentTypeEnd);
        data = data.mid(employmentType.size() + data.indexOf(employmentType));
    }

    description.replace("<br>", "\n");
    description.replace("<p>", "\n");
    description.replace("<ol>", "\n");
    description.replace("<ul>", "\n");
    description.replace("<li>", "\n");
    description.replace("&nbsp;", " ");

    description.remove("</p>");
    description.remove("<strong>");
    description.remove("</strong>");
    description.remove("<span>");
    description.remove("</span>");
    description.remove("</li>");
    description.remove("</ul>");
    description.remove("</ol>");
    description.remove("<em>");
    description.remove("</em>");
    description.remove("<u>");
    description.remove("</u>");

    LinkedApp::removeSpaces(description);
    description.replace("\n", "\n" + QString(17, ' ') + "| ");
}

void LinkedVacancy::print(QTextStream &stream) const
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

QString LinkedVacancy::extract(QString data, const QString &start, const QString &end)
{
    auto firstIndex = data.indexOf(start);
    if (firstIndex == -1)
    {
        return QString();
    }

    data = data.mid(firstIndex + start.size());
    auto secondIndex = data.indexOf(end);
    if (secondIndex == -1)
    {
        return QString();
    }

    return data.mid(0, secondIndex);
}

qsizetype LinkedVacancy::count(const QString &str, Qt::CaseSensitivity cs) const
{
    return title.count(str, cs) + description.count(str, cs);
}

void LinkedVacancy::clear()
{
    title = QString();
    link = QString();
    companyName = QString();
    companyLink = QString();
    location = QString();
    date = QDate();
    description = QString();
}