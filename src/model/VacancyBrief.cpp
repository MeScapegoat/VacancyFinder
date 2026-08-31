#include "VacancyBrief.h"

#include "../utils/Utils.h"


Q_LOGGING_CATEGORY(VacancyBriefLog, "vacancy.brief");


VacancyBrief::VacancyBrief() :
    id(0),
    title(),
    link(),
    companyName(),
    companyLink(),
    location(),
    date()
{
    qCDebug(VacancyBriefLog) << __FUNCTION__;
}


VacancyBrief::~VacancyBrief()
{
    qCDebug(VacancyBriefLog) << __FUNCTION__;
}

QDebug operator<<(QDebug debug, const VacancyBrief& vacancyBrief)
{
    QDebugStateSaver saver(debug);

    debug.noquote() << "VacancyBrief:\n";
    print(debug, "id",          vacancyBrief.id);
    print(debug, "title",       vacancyBrief.title);
    print(debug, "link",        vacancyBrief.link);
    print(debug, "companyName", vacancyBrief.companyName);
    print(debug, "companyLink", vacancyBrief.companyLink);
    print(debug, "location",    vacancyBrief.location);
    print(debug, "date",        vacancyBrief.date);

    return debug;
}
