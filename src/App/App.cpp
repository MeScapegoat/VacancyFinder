#include "App.h"

#include <QDir>
#include <QFile>
#include <QListView>
#include <QStackedLayout>
#include <QTextBrowser>

#include <LinkedInVacancySearchWorkflow.h>
#include <MainWindow.h>
#include <ProgressDialog.h>
#include <QueryParametersPresenter.h>
#include <ResultViewOptionsPresenter.h>
#include <VacancyBrowserPresenter.h>
#include <VacancySearchWorkflow.h>


Q_LOGGING_CATEGORY(AppLog, "app");

App::App(QObject* parent) :
    QObject(parent),
    queryParametersPresenter(*new QueryParametersPresenter(this)),
    resultViewOptionsPresenter(*new ResultViewOptionsPresenter(this)),
    vacancyBrowserPresenter(*new VacancyBrowserPresenter(this)),
    vacancySearchWorkflow(*new LinkedInVacancySearchWorkflow(this)),
    mainWindow(*new MainWindow(queryParametersPresenter.viewWidget(), resultViewOptionsPresenter.viewWidget(), vacancyBrowserPresenter.viewWidget())),
    progressDialog(*new ProgressDialog(&mainWindow))
{
    // QueryParameters
    QObject::connect(&queryParametersPresenter, &QueryParametersPresenter::searchRequested, this, &App::sendSearchRequest);
    QObject::connect(&queryParametersPresenter, &QueryParametersPresenter::loadInterestedRequested, this, &App::loadInterested);
    QObject::connect(&queryParametersPresenter, &QueryParametersPresenter::resetRequested, this, &App::reset);
    QObject::connect(&queryParametersPresenter, &QueryParametersPresenter::keywordsChanged, &resultViewOptionsPresenter, &ResultViewOptionsPresenter::setKeywords);


    // ResultViewOptions
    QObject::connect(&resultViewOptionsPresenter, &ResultViewOptionsPresenter::vacancyFilterChanged,         &vacancyBrowserPresenter, &VacancyBrowserPresenter::setVacancyFilter);
    QObject::connect(&resultViewOptionsPresenter, &ResultViewOptionsPresenter::visibleStatusesChanged,       &vacancyBrowserPresenter, &VacancyBrowserPresenter::setVisibleStatuses);
    QObject::connect(&resultViewOptionsPresenter, &ResultViewOptionsPresenter::vacancySortComparatorChanged, &vacancyBrowserPresenter, &VacancyBrowserPresenter::setVacancySortComparator);

    QObject::connect(&resultViewOptionsPresenter, &ResultViewOptionsPresenter::resultViewOptionsChanged,     &vacancyBrowserPresenter, &VacancyBrowserPresenter::updateVisibleVacancies);

    vacancyBrowserPresenter.setVacancyFilter(resultViewOptionsPresenter.vacancyFilter());
    vacancyBrowserPresenter.setVisibleStatuses(resultViewOptionsPresenter.visibleStatuses());
    vacancyBrowserPresenter.setVacancySortComparator(resultViewOptionsPresenter.vacancySortComparator());


    // ProgressDialog
    QObject::connect(&progressDialog.stopButton, &QPushButton::released, &vacancySearchWorkflow, &VacancySearchWorkflow::interrupt);

    // VacancySearchWorkflow
    QObject::connect(&vacancySearchWorkflow, &VacancySearchWorkflow::vacancyCountDetermined, &progressDialog, &ProgressDialog::updateVacancyCount);
    QObject::connect(&vacancySearchWorkflow, &VacancySearchWorkflow::briefReady, &progressDialog, &ProgressDialog::updateBrief);
    QObject::connect(&vacancySearchWorkflow, &VacancySearchWorkflow::detailsReady, &progressDialog, &ProgressDialog::updateDetails);
    QObject::connect(&vacancySearchWorkflow, &VacancySearchWorkflow::vacanciesReady, &vacancyBrowserPresenter, &VacancyBrowserPresenter::onVacanciesReady);
    QObject::connect(&vacancySearchWorkflow, &VacancySearchWorkflow::allActiveWorkFinished, &progressDialog, &ProgressDialog::accept);


    QObject::connect(this, &App::openProgressDialog, &progressDialog, &ProgressDialog::start);

    QObject::connect(this, &App::reset, &vacancyBrowserPresenter, &VacancyBrowserPresenter::reset);
    QObject::connect(this, &App::reset, &vacancySearchWorkflow, &VacancySearchWorkflow::reset);

    
    loadState();
}

App::~App()
{
    saveState();
}

void App::loadInterested()
{
    const auto& interestedVacancies = vacancyBrowserPresenter.interestedVacancies();

    updateMaxVacancies(interestedVacancies.size());


    vacancySearchWorkflow.startSearch(interestedVacancies.values());
    emit openProgressDialog(maxVacancies);
}

void App::sendSearchRequest()
{    
    updateMaxVacancies(queryParametersPresenter.maxResults());

    keywords = queryParametersPresenter.keywords();
    location = queryParametersPresenter.location();

    qCInfo(AppLog) << "keywords:" << keywords << "location:" << location;


    vacancySearchWorkflow.startSearch(keywords, location, maxVacancies);
    emit openProgressDialog(maxVacancies);
}

void App::updateMaxVacancies(const int maxVacancies_)
{
    maxVacancies = maxVacancies_;
}

void App::saveState()
{
    state.setIgnoredVacancies(vacancyBrowserPresenter.ignoredVacancies());
    state.setInterestedVacancies(vacancyBrowserPresenter.interestedVacancies());


    state.setKeywords(queryParametersPresenter.keywords());
    state.setLocation(queryParametersPresenter.location());

    state.setMaxResults(queryParametersPresenter.maxResults());


    state.setFilterMode(resultViewOptionsPresenter.filterMode());
    state.setFilters(resultViewOptionsPresenter.filterWords());

    state.setSortMode(resultViewOptionsPresenter.sortMode());

    state.setIsUnreviewedVisible(resultViewOptionsPresenter.isUnreviewedVisible());
    state.setIsIgnoredVisible(resultViewOptionsPresenter.isIgnoredVisible());
    state.setIsInterestedVisible(resultViewOptionsPresenter.isInterestedVisible());


    state.saveState();
}

void App::loadState()
{
    state.loadState();

    vacancyBrowserPresenter.setIgnoredVacancies(state.ignoredVacancies());
    vacancyBrowserPresenter.setInterestedVacancies(state.interestedVacancies());


    queryParametersPresenter.setKeywords(state.keywords());
    queryParametersPresenter.setLocation(state.location());

    queryParametersPresenter.setMaxResults(state.maxResults());


    resultViewOptionsPresenter.setFilterMode(state.filterMode());
    resultViewOptionsPresenter.setFilterWords(state.filters());

    resultViewOptionsPresenter.setSortMode(state.sortMode());

    resultViewOptionsPresenter.setIsUnreviewedVisible(state.isUnreviewedVisible());
    resultViewOptionsPresenter.setIsIgnoredVisible(state.isIgnoredVisible());
    resultViewOptionsPresenter.setIsInterestedVisible(state.isInterestedVisible());
}
