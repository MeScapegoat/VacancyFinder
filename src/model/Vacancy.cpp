#include "Vacancy.h"


Q_LOGGING_CATEGORY(VacancyLog, "vacancy");


Vacancy::Vacancy() :
    brief(),
    details()
{
    qCDebug(VacancyLog) << __FUNCTION__;
}

Vacancy::Vacancy(const VacancyBrief& _brief) :
    brief(_brief),
    details()
{
    qCDebug(VacancyLog) << __FUNCTION__;
}

Vacancy::Vacancy(const VacancyDetails& _details) :
    brief(),
    details(_details)
{
    qCDebug(VacancyLog) << __FUNCTION__;
}

Vacancy::Vacancy(const VacancyBrief& _brief, const VacancyDetails& _details) :
    brief(_brief),
    details(_details)
{
    qCDebug(VacancyLog) << __FUNCTION__;
}

Vacancy::~Vacancy()
{
    qCDebug(VacancyLog) << __FUNCTION__;
}

qsizetype Vacancy::count(const QString& str, Qt::CaseSensitivity cs) const
{
    qsizetype counted = 0;

    counted += brief.title.count(str, cs);

    counted += details.description.count(str, cs);

    return counted;
}

QDebug operator<<(QDebug debug, const Vacancy& vacancy)
{

    QDebugStateSaver saver(debug);

    debug.nospace() << "Vacancy:\n"
        << vacancy.brief
        << vacancy.details;

    return debug;
}