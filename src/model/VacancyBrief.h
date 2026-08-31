#pragma once

#include <QDate>
#include <QLoggingCategory>
#include <QString>
#include <QUrl>


Q_DECLARE_LOGGING_CATEGORY(VacancyBriefLog);


class VacancyBrief
{
public:
    VacancyBrief();
    ~VacancyBrief();

public:
    unsigned id;

    QString title;
    QUrl link;

    QString companyName;
    QUrl companyLink;

    QString location;

    QDate date;
};

QDebug operator<<(QDebug debug, const VacancyBrief& vacancyBrief);