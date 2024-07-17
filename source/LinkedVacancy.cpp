#include <iostream> //tmp

#include "LinkedVacancy.h"
LinkedVacancy::LinkedVacancy(const QString &data)
{
    load(data);
}

void LinkedVacancy::load(QString data)
{
    QString linkStart = "href=\"", linkEnd = ";";
    QString titleStart = "class=\"base-search-card__title\">\n", titleEnd = "\n";
    QString companyLinkStart = "href=\"", companyLinkEnd = "\"";
    QString companyNameStart = ">\n", companyNameEnd = "\n";
    QString locationStart = "class=\"job-search-card__location\">\n", locationEnd = "\n";
    QString publishTimeStart = "datetime=\"", publishTimeEnd = "\"";

    link = extract(data, linkStart, linkEnd);
    data = data.mid(link.size() + data.indexOf(link));

    title = extract(data, titleStart, titleEnd);
    data = data.mid(title.size() + data.indexOf(title));

    companyLink = extract(data, companyLinkStart, companyLinkEnd);
    data = data.mid(companyLink.size() + data.indexOf(companyLink));

    companyName = extract(data, companyNameStart, companyNameEnd);
    data = data.mid(companyName.size() + data.indexOf(companyName));

    location = extract(data, locationStart, locationEnd);
    data = data.mid(location.size() + data.indexOf(location));

    QString dateStr = extract(data, publishTimeStart, publishTimeEnd);
    date = QDate::fromString(dateStr, "YYYY-MM-DD");
    data = data.mid(dateStr.size() + data.indexOf(dateStr));
}

void LinkedVacancy::print(QTextStream &stream) const
{
    stream << "title:       " << title << "\n";
    stream << "Link:        " << link << "\n";
    stream << "companyName: " << companyName << "\n";
    stream << "companyLink: " << companyLink << "\n";
    stream << "location:    " << location << "\n";
    stream << "date       : " << date.toString("DD-MM-YYYY") << "\n";
    stream << "description: " << description << "\n";
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

qsizetype LinkedVacancy::count(const QString& str, Qt::CaseSensitivity cs) const
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