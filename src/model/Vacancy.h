#pragma once
#include <QLoggingCategory>

#include "VacancyBrief.h"
#include "VacancyDetails.h"


Q_DECLARE_LOGGING_CATEGORY(VacancyLog);


class Vacancy
{
public:
    Vacancy();
    Vacancy(const VacancyBrief& brief);
    Vacancy(const VacancyDetails& details);
    Vacancy(const VacancyBrief& brief, const VacancyDetails& details);

    ~Vacancy();

public:
    qsizetype count(const QString& str, Qt::CaseSensitivity cs = Qt::CaseInsensitive) const;

public:
    VacancyBrief brief;
    VacancyDetails details;
};

QDebug operator<<(QDebug debug, const Vacancy& vacancy);

