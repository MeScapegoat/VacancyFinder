#include "LinkedInVacancySearchWorkflow.h"

#include <AbstractParserStrategy.h>
#ifdef NDEBUG
#include <LinkedInParserStrategy/LinkedInParserStrategy.h>
#else
#include <LinkedInParserStrategy/LinkedInPageCollector.h>
#endif
#include <LinkedInRequestStrategy.h>
#include <LinkedInSearchUrlProvider.h>
#include <NetworkManager.h>


namespace
{
    AbstractParserStrategy* createParserStrategy()
    {
#ifdef NDEBUG
        return new LinkedInParserStrategy();
#else
        return new LinkedInPageCollector();
#endif
    }
}

LinkedInVacancySearchWorkflow::LinkedInVacancySearchWorkflow(QObject* parent) :
    VacancySearchWorkflow(new LinkedInSearchUrlProvider,
                          new LinkedInRequestStrategy(new NetworkManager),
                          createParserStrategy(),
                          parent)
{
}
