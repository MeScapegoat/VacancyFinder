#include <iostream> //tmp

#include "LinkedVacancy.h"
LinkedVacancy::LinkedVacancy(const QString &data)
{
    load(data);
}

bool LinkedVacancy::load(QString data)
{
    link = extract(data, "href=\"", ";");
    //std::cout << "link: " << link.toStdString() << "\n";
    data = data.mid(link.size() + data.indexOf(link));

    header = extract(data, "class=\"sr-only\">\n", "\n");
    //std::cout << "header: " << header.toStdString() << "\n";
    data = data.mid(header.size());

    //std::cout << "\n";
    return true;
}

void LinkedVacancy::print(QTextStream &stream)
{
    /*
    QString header;
    QString link;
    QString companyName;
    QString companyLink;
    QString description;
    QString publishTime;
    QString country;
    QString city;
    */
    stream << "Header: " << header << "\n";
    stream << "Link: " << link << "\n";
    stream << "companyName: " << companyName << "\n";
    stream << "companyLink: " << companyLink << "\n";
    stream << "description: " << description << "\n";
    stream << "publishTime: " << publishTime << "\n";
    stream << "country: " << country << "\n";
    stream << "city: " << city << "\n";
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