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
#include <Parser.h>
#include <RequestManager.h>


namespace
{
    AbstractParserStrategy& createParserStrategy()
    {
#ifdef NDEBUG
        return *new LinkedInParserStrategy();
#else
        return *new LinkedInPageCollector();
#endif
    }

    AbstractSearchUrlProvider& createSearchUrlProvider()
    {
        return *new LinkedInSearchUrlProvider;
    }

    RequestManager& createRequestManager()
    {
        return *new RequestManager(*new LinkedInRequestStrategy(new NetworkManager));
    }

    Parser& createParser()
    {
        return *new Parser(createParserStrategy());
    }
}

LinkedInVacancySearchWorkflow::LinkedInVacancySearchWorkflow(QObject* parent) :
    VacancySearchWorkflow(createSearchUrlProvider(), createRequestManager(), createParser(), parent)
{}
