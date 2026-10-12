#include "App.h"

#include <QDir>
#include <QFile>
#include <QListView>
#include <QStackedLayout>
#include <QTextBrowser>
#include <QThread>
#include <QUrlQuery>

#include "AppController.h"
#include <MainWindow.h>
#include <NetworkManager.h>
#include <Parser.h>
#include <ProgressDialog.h>
#include <QueryParametersPresenter.h>
#include <RequestManager.h>
#include <RequestStrategy/LinkedInRequestStrategy.h>
#include <ResultViewOptionsPresenter.h>
#include <VacancyBrowserPresenter.h>
#include <Vacancy.h>

#ifdef NDEBUG
#include <LinkedInParserStrategy/LinkedInParserStrategy.h>
#else
#include <LinkedInParserStrategy/LinkedInPageCollector.h>
#endif


namespace
{
    inline constexpr const char* BaseUrl = "https://www.linkedin.com/jobs-guest/jobs/api/seeMoreJobPostings/search";
}

Q_LOGGING_CATEGORY(AppLog, "app");

App::App(QObject* parent) :
    QObject(parent),
    queryParametersPresenter(*new QueryParametersPresenter(this)),
    resultViewOptionsPresenter(*new ResultViewOptionsPresenter(this)),
    vacancyBrowserPresenter(*new VacancyBrowserPresenter(this)),
    controller(*new AppController(this)),
    mainWindow(*new MainWindow(queryParametersPresenter.getView().widget(), resultViewOptionsPresenter.getView().widget(), vacancyBrowserPresenter.getView().widget())),
    progressDialog(*new ProgressDialog(&mainWindow)),
    requestStrategy(new LinkedInRequestStrategy(new NetworkManager)),
    requestManager(*new RequestManager(requestStrategy)),
    networkThread(*new QThread(this)),
    parser(*new Parser),
    parserThread(*new QThread(this)),
#ifdef NDEBUG
    parserStrategy(new LinkedInParserStrategy)
#else
    parserStrategy(new LinkedInPageCollector)
#endif
{
    requestStrategy->setParent(&requestManager); // TODO: improve ownership
    parser.setStrategy(parserStrategy.get());

    // Threads
    networkThread.setObjectName("networkThread");
    requestManager.moveToThread(&networkThread);

    parserThread.setObjectName("parserThread");
    parser.moveToThread(&parserThread);

    QObject::connect(&networkThread, &QThread::finished, &requestManager, &RequestManager::deleteLater);
    QObject::connect(&parserThread, &QThread::finished, &parser, &Parser::deleteLater);


    // QueryParameters
    QObject::connect(&queryParametersPresenter, &QueryParametersPresenter::searchRequested, this, &App::sendSearchRequest);
    QObject::connect(&queryParametersPresenter, &QueryParametersPresenter::loadInterestedRequested, this, &App::loadInterested);
    QObject::connect(&queryParametersPresenter, &QueryParametersPresenter::resetRequested, this, &App::reset);


    // ResultViewOptions
    QObject::connect(&resultViewOptionsPresenter, &ResultViewOptionsPresenter::resultViewOptionsChanged, &vacancyBrowserPresenter, &VacancyBrowserPresenter::updateVisibleVacancies);

    // TODO: TMP CODE, REMOVE
    auto& VBM = vacancyBrowserPresenter.getModel();
    QObject::connect(&resultViewOptionsPresenter, &ResultViewOptionsPresenter::sortModeChanged,              &VBM, &VacancyBrowserModel::setSortMode);
    QObject::connect(&resultViewOptionsPresenter, &ResultViewOptionsPresenter::filterModeChanged,            &VBM, &VacancyBrowserModel::setFilterMode);
    QObject::connect(&resultViewOptionsPresenter, &ResultViewOptionsPresenter::filterWordsChanged,           &VBM, &VacancyBrowserModel::setFilterWords);
    QObject::connect(&resultViewOptionsPresenter, &ResultViewOptionsPresenter::isUnreviewedVsisibleChanged,  &VBM, &VacancyBrowserModel::setIsUnreviewedVisible);
    QObject::connect(&resultViewOptionsPresenter, &ResultViewOptionsPresenter::isIgnoredVsisibleChanged,     &VBM, &VacancyBrowserModel::setIsIgnoredVisible);
    QObject::connect(&resultViewOptionsPresenter, &ResultViewOptionsPresenter::isInterestedVsisibleChanged,  &VBM, &VacancyBrowserModel::setIsInterestedVisible);
    QObject::connect(&queryParametersPresenter,   &QueryParametersPresenter::keywordsChanged,                &VBM, &VacancyBrowserModel::setKeywords);

    // RequestManager
    QObject::connect(&requestManager, &RequestManager::requestsStarted, &controller, &AppController::workStarted);
    QObject::connect(&requestManager, &RequestManager::requestsFinished, &controller, &AppController::workFinished);

    //QObject::connect(&requestManager, &RequestManager::meaningfulRepliesReceived, this, &App::onMeaningfulRepliesReceived); // -1 network work +1 parsing work, so the total work remains unchanged
    QObject::connect(&requestManager, &RequestManager::meaninglessRepliesReceived, &controller, &AppController::workFinished);
    
    QObject::connect(&requestManager, &RequestManager::receivedReply, &parser, &Parser::processPage);
    QObject::connect(&requestManager, &RequestManager::vacancyCountDetermined, &progressDialog, &ProgressDialog::updateVacancyCount);

    QObject::connect(&requestManager, &RequestManager::urlsEnqueued, &controller, &AppController::workStarted);
    QObject::connect(&requestManager, &RequestManager::urlsDequeued, &controller, &AppController::workFinished);


    // Parser
    QObject::connect(&parser, &Parser::requestDetails, &requestManager, &RequestManager::addUrl);

    //QObject::connect(&parser, &Parser::briefParsed, this, &App::onBriefReady);
    QObject::connect(&parser, &Parser::briefParsed, &vacancyBrowserPresenter, &VacancyBrowserPresenter::onBriefReady);
    QObject::connect(&parser, &Parser::briefParsed, &progressDialog, &ProgressDialog::updateBrief);

    //QObject::connect(&parser, &Parser::detailsParsed, this, &App::onDetailsReady);
    QObject::connect(&parser, &Parser::detailsParsed, &vacancyBrowserPresenter, &VacancyBrowserPresenter::onDetailsReady);
    QObject::connect(&parser, &Parser::detailsParsed, &progressDialog, &ProgressDialog::updateDetails);

    //QObject::connect(&parser, &Parser::pageParsingStarted,  &controller, &AppController::workStarted); // Work is actually added in App::onMeaningfulRepliesReceived
    QObject::connect(&parser, &Parser::pageParsingFinished, &controller, &AppController::workFinished);


    // ProgressDialog
    QObject::connect(&progressDialog.stopButton, &QPushButton::released, &requestManager, &RequestManager::interrupt);
    QObject::connect(&progressDialog.stopButton, &QPushButton::released, &parser, &Parser::interrupt);

    // AppController
    QObject::connect(&controller, &AppController::allActiveWorkFinished, &vacancyBrowserPresenter, &VacancyBrowserPresenter::updateVisibleVacancies);
    QObject::connect(&controller, &AppController::allActiveWorkFinished, &progressDialog, &ProgressDialog::accept);

    // This
    QObject::connect(this, &App::initSearch, &requestManager, &RequestManager::addUrls);    
    QObject::connect(this, &App::openProgressDialog, &progressDialog, &ProgressDialog::start);

    
    loadState();

    networkThread.start();
    parserThread.start();
}

App::~App()
{
    saveState();

    networkThread.quit();
    networkThread.wait();

    parserThread.quit();
    parserThread.wait();
}

void App::reset()
{
    VacancyBrowserModel& model = vacancyBrowserPresenter.getModel();

    model.vacancyByUrl().clear();
    model.visibleVacancies().clear();

    controller.reset();
    requestManager.reset();
    parser.reset();
}

void App::loadInterested()
{
    const auto& interestedVacancies = vacancyBrowserPresenter.getModel().interestedVacancies();

    updateMaxVacancies(interestedVacancies.size());

    searchStartTime = QTime::currentTime();

    emit initSearch(interestedVacancies.values().toVector());
    emit openProgressDialog(maxVacancies);
}

void App::sendSearchRequest()
{    
    updateMaxVacancies(queryParametersPresenter.getModel().maxResults());

    controller.reset();

    keywords = queryParametersPresenter.getModel().keywords();
    location = queryParametersPresenter.getModel().location();

    qCInfo(AppLog) << "keywords:" << keywords << "location:" << location;

    QVector<QUrl> urls;

    for (auto n = 0; n < maxVacancies; n += 10)
    {
        QUrl url(BaseUrl);
        QUrlQuery query;
        query.addQueryItem("keywords", QUrl::toPercentEncoding(keywords));
        query.addQueryItem("location", QUrl::toPercentEncoding(location));
        query.addQueryItem("start", QString::number(n));
        url.setQuery(query);

        urls.append(url);
    }

    searchStartTime = QTime::currentTime();

    emit initSearch(urls);
    emit openProgressDialog(maxVacancies);
}

void App::updateMaxVacancies(const int maxVacancies_)
{
    maxVacancies = maxVacancies_;
}

void App::onMeaningfulRepliesReceived(int amount)
{
    controller.workStarted(amount);
    controller.workFinished(amount);
}

QString App::loadTitleFromFile(const QString& path)
{
    QFile file(path);
    QString ret;
    if (file.open(QIODeviceBase::ReadOnly))
    {
        ret = file.readAll();
        file.close();
    }
    else
        qCWarning(AppLog) << "couldn't open " << path;
    return ret;
}

QString App::loadDescriptionFromFile(const QString& path)
{
    QFile file(path);
    QString ret;
    if (file.open(QIODeviceBase::ReadOnly))
    {
        ret = file.readAll();
        file.close();
    }
    else
        qCWarning(AppLog) << "couldn't open " << path;
    return ret;
}

void App::saveState()
{
    state.setIgnored(vacancyBrowserPresenter.getModel().ignoredVacancies());
    state.setInterested(vacancyBrowserPresenter.getModel().interestedVacancies());

    state.setKeywords(queryParametersPresenter.getModel().keywords());
    state.setLocation(queryParametersPresenter.getModel().location());

    state.setLimit(queryParametersPresenter.getModel().maxResults());

    state.setFilterMode(resultViewOptionsPresenter.getModel().currentFilterMode());
    state.setFilters(resultViewOptionsPresenter.getModel().filterText());

    state.setSortMode(resultViewOptionsPresenter.getModel().currentSortMode());

    state.setIsUnreviewedVisible(resultViewOptionsPresenter.getModel().isUnreviewedVisible());
    state.setIsIgnoredVisible(resultViewOptionsPresenter.getModel().isIgnoredVisible());
    state.setIsInterestedVisible(resultViewOptionsPresenter.getModel().isInterestedVisible());


    state.saveState();
}

void App::loadState()
{
    state.loadState();

    vacancyBrowserPresenter.getModel().setIgnoredVacancies(state.getIgnored());
    vacancyBrowserPresenter.getModel().setInterestedVacancies(state.getInterested());

    queryParametersPresenter.getModel().keywords = state.getKeywords();
    queryParametersPresenter.getModel().location = state.getLocation();

    queryParametersPresenter.getModel().maxResults = state.getLimit();

    resultViewOptionsPresenter.getModel().currentFilterMode = state.getFilterMode();
    resultViewOptionsPresenter.getModel().filterText = state.getFilters();

    resultViewOptionsPresenter.getModel().currentSortMode = state.getSortMode();


    resultViewOptionsPresenter.getModel().isUnreviewedVisible = state.getIsUnreviewedVisible();
    resultViewOptionsPresenter.getModel().isIgnoredVisible = state.getIsIgnoredVisible();
    resultViewOptionsPresenter.getModel().isInterestedVisible = state.getIsInterestedVisible();
}