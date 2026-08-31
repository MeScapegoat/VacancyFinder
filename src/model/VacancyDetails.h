#pragma once

#include <optional>

#include <QDate>
#include <QLoggingCategory>
#include <QString>
#include <QUrl>


Q_DECLARE_LOGGING_CATEGORY(VacancyDetailsLog);


using Months = unsigned;

class VacancyDetails
{
public:
    VacancyDetails();
    ~VacancyDetails();

public:
    unsigned id;

    QUrl link;

    QString employmentType;

    QString skills;

    QString description;


    std::optional<QString> industry;

    std::optional<QString> level;
    std::optional<Months> experience;

    std::optional<QString> education;

    std::optional<QDate> expirationDate;
};

QDebug operator<<(QDebug debug, const VacancyDetails& vacancyDetails);