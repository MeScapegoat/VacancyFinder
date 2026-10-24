#include "VacancySearchWorkflow.h"

#include <AbstractParserStrategy.h>
#include <AbstractRequestStrategy.h>
#include <AbstractSearchUrlProvider.h>
#include <Parser.h>
#include <RequestManager.h>
#include <WorkCoordinator.h>


VacancySearchWorkflow::VacancySearchWorkflow(AbstractSearchUrlProvider* searchUrlProvider,
                                             AbstractRequestStrategy* requestStrategy,
                                             AbstractParserStrategy* parserStrategy,
                                             QObject* parent) :
    QObject(parent),
    m_searchUrlProvider(searchUrlProvider),
    m_requestManager(*new RequestManager(requestStrategy)),
    m_parser(*new Parser(parserStrategy)),
    m_workCoordinator(*new WorkCoordinator(this)),
    m_networkThread(*new QThread(this)),
    m_parserThread(*new QThread(this)),
    m_vacancyByUrl()
{
    searchUrlProvider->setParent(this);

    Q_ASSERT_X(searchUrlProvider, __FUNCTION__, "searchUrlProvider is nullptr");

    m_networkThread.setObjectName("networkThread");
    m_requestManager.moveToThread(&m_networkThread);

    m_parserThread.setObjectName("parserThread");
    m_parser.moveToThread(&m_parserThread);


    QObject::connect(&m_networkThread, &QThread::finished, &m_requestManager, &RequestManager::deleteLater);
    QObject::connect(&m_parserThread, &QThread::finished, &m_parser, &Parser::deleteLater);

    
    QObject::connect(&m_requestManager, &RequestManager::requestsStarted,            &m_workCoordinator, &WorkCoordinator::workStarted);
    QObject::connect(&m_requestManager, &RequestManager::requestsFinished,           &m_workCoordinator, &WorkCoordinator::workFinished);
    
    // RequestManager::meaningfulRepliesReceived is not connected due to optimization
    QObject::connect(&m_requestManager, &RequestManager::meaninglessRepliesReceived, &m_workCoordinator, &WorkCoordinator::workFinished);

    QObject::connect(&m_requestManager, &RequestManager::urlsEnqueued,               &m_workCoordinator, &WorkCoordinator::workStarted);
    QObject::connect(&m_requestManager, &RequestManager::urlsDequeued,               &m_workCoordinator, &WorkCoordinator::workFinished);

    QObject::connect(&m_requestManager, &RequestManager::receivedReply,              &m_parser, &Parser::processPage);
    QObject::connect(&m_requestManager, &RequestManager::vacancyCountDetermined,     this, &VacancySearchWorkflow::vacancyCountDetermined);


    QObject::connect(&m_parser, &Parser::requestDetails,        &m_requestManager, &RequestManager::addUrl);

    // Parser::pageParsingStarted is not connected due to optimization
    QObject::connect(&m_parser, &Parser::pageParsingFinished,   &m_workCoordinator, &WorkCoordinator::workFinished);

    QObject::connect(&m_parser, &Parser::briefParsed,           this, &VacancySearchWorkflow::onBriefParsed);
    QObject::connect(&m_parser, &Parser::detailsParsed,         this, &VacancySearchWorkflow::onDetailsParsed);


    QObject::connect(this, &VacancySearchWorkflow::interrupt, &m_requestManager, &RequestManager::interrupt);
    QObject::connect(this, &VacancySearchWorkflow::interrupt, &m_parser, &Parser::interrupt);

    QObject::connect(this, &VacancySearchWorkflow::reset, this, &VacancySearchWorkflow::onReset);
    QObject::connect(this, &VacancySearchWorkflow::reset, &m_requestManager, &RequestManager::reset);
    QObject::connect(this, &VacancySearchWorkflow::reset, &m_parser, &Parser::reset);

    QObject::connect(this, &VacancySearchWorkflow::urlsRequested, &m_requestManager, &RequestManager::addUrls);


    QObject::connect(&m_workCoordinator, &WorkCoordinator::allActiveWorkFinished, this, &VacancySearchWorkflow::onWorkFinished);


    m_networkThread.start();
    m_parserThread.start();
}

VacancySearchWorkflow::~VacancySearchWorkflow()
{
    m_networkThread.quit();
    m_networkThread.wait();

    m_parserThread.quit();
    m_parserThread.wait();
}

void VacancySearchWorkflow::startSearch(const QList<QUrl> urls)
{
    emit urlsRequested(urls);
}

void VacancySearchWorkflow::startSearch(const QString& keywords, const QString& location, qsizetype maxVacancies)
{
    const auto urls = m_searchUrlProvider->createSearchUrls(keywords, location, maxVacancies);
    emit urlsRequested(urls);
}

void VacancySearchWorkflow::storeVacancy(const Vacancy& vacancy)
{
    const auto it = m_vacancyByUrl.find(vacancy.url);
    if (it == m_vacancyByUrl.end())
    {
        m_vacancyByUrl.emplace(vacancy.url, vacancy);
    }
    else
    {
        Vacancy& foundVacancy = it.value();
        foundVacancy.fillEmptyFrom(vacancy);
    }
}

void VacancySearchWorkflow::onBriefParsed(const Vacancy& vacancy)
{
    storeVacancy(vacancy);

    emit briefReady(vacancy);
}

void VacancySearchWorkflow::onDetailsParsed(const Vacancy& vacancy)
{
    storeVacancy(vacancy);

    emit detailsReady(vacancy);
}

void VacancySearchWorkflow::onWorkFinished()
{
    emit vacanciesReady(std::move(m_vacancyByUrl.values()));
    emit allActiveWorkFinished();
}

void VacancySearchWorkflow::onReset()
{
    m_vacancyByUrl.clear();

    m_workCoordinator.reset();
}
