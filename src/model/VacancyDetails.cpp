#include "VacancyDetails.h"

#include "../utils/Utils.h"

Q_LOGGING_CATEGORY(VacancyDetailsLog, "vacancy.details");


VacancyDetails::VacancyDetails() :
    id(0),
    link(),
    level(),
    employmentType(),
    description()
{
    qCDebug(VacancyDetailsLog) << __FUNCTION__;
}

VacancyDetails::~VacancyDetails()
{
    qCDebug(VacancyDetailsLog) << __FUNCTION__;
}

QDebug operator<<(QDebug debug, const VacancyDetails& vacancyDetails)
{
    QDebugStateSaver saver(debug);

    debug.noquote() << "VacancyDetails:\n";

    print(debug, "id",             vacancyDetails.id);
    print(debug, "link",           vacancyDetails.link);
    print(debug, "level",          vacancyDetails.level);
    print(debug, "employmentType", vacancyDetails.employmentType);
    print(debug, "description",    vacancyDetails.description);

    print(debug, "industry",       vacancyDetails.industry);
    print(debug, "expirationDate", vacancyDetails.expirationDate);
    print(debug, "experience",     vacancyDetails.experience);
    print(debug, "education",      vacancyDetails.education);
    print(debug, "skills",         vacancyDetails.skills);
    
    return debug;
}
