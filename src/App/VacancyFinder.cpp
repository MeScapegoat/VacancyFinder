#include "VacancyFinder.h"

#include <QDir>
#include <QFile>
#include <QListView>
#include <QLoggingCategory>
#include <QStackedLayout>
#include <QTextBrowser>

#include <LinkedInVacancySearchWorkflow.h>
#include <MainWindow.h>
#include <ProgressDialog.h>
#include <QueryParametersPresenter.h>
#include <ResultViewOptionsPresenter.h>
#include <VacancyBrowserPresenter.h>
#include <VacancySearchWorkflow.h>


Q_LOGGING_CATEGORY(VacancyFinderLog, "vacancyFinder");

VacancyFinder::VacancyFinder(QObject* parent) :
    QObject(parent),
    queryParametersPresenter(*new QueryParametersPresenter(this)),
    resultViewOptionsPresenter(*new ResultViewOptionsPresenter(this)),
    vacancyBrowserPresenter(*new VacancyBrowserPresenter(this)),
    vacancySearchWorkflow(*new LinkedInVacancySearchWorkflow(this)),
    mainWindow(*new MainWindow(queryParametersPresenter.viewWidget(), resultViewOptionsPresenter.viewWidget(), vacancyBrowserPresenter.viewWidget())),
    progressDialog(*new ProgressDialog(&mainWindow))
{
    // QueryParameters
    QObject::connect(&queryParametersPresenter, &QueryParametersPresenter::searchRequested, this, &VacancyFinder::sendSearchRequest);
    QObject::connect(&queryParametersPresenter, &QueryParametersPresenter::loadInterestedRequested, this, &VacancyFinder::loadInterested);
    QObject::connect(&queryParametersPresenter, &QueryParametersPresenter::keywordsChanged, &resultViewOptionsPresenter, &ResultViewOptionsPresenter::setKeywords);
    QObject::connect(&vacancyBrowserPresenter, &VacancyBrowserPresenter::interestedVacanciesChanged, this, &VacancyFinder::updateLoadInterestedButtonVisibility);


    // ResultViewOptions
    QObject::connect(&resultViewOptionsPresenter, &ResultViewOptionsPresenter::vacancyFilterChanged,              &vacancyBrowserPresenter, &VacancyBrowserPresenter::setVacancyFilter);
    QObject::connect(&resultViewOptionsPresenter, &ResultViewOptionsPresenter::visibleStatusesChanged,            &vacancyBrowserPresenter, &VacancyBrowserPresenter::setVisibleStatuses);
    QObject::connect(&resultViewOptionsPresenter, &ResultViewOptionsPresenter::vacancySortComparatorChanged,      &vacancyBrowserPresenter, &VacancyBrowserPresenter::setVacancySortComparator);
    QObject::connect(&resultViewOptionsPresenter, &ResultViewOptionsPresenter::ignoredCompaniesVisibilityChanged, &vacancyBrowserPresenter, &VacancyBrowserPresenter::setIgnoredCompaniesVisible);

    QObject::connect(&resultViewOptionsPresenter, &ResultViewOptionsPresenter::resultViewOptionsChanged,     &vacancyBrowserPresenter, &VacancyBrowserPresenter::updateVisibleVacancies);

    vacancyBrowserPresenter.setVacancyFilter(resultViewOptionsPresenter.vacancyFilter());
    vacancyBrowserPresenter.setVisibleStatuses(resultViewOptionsPresenter.visibleStatuses());
    vacancyBrowserPresenter.setVacancySortComparator(resultViewOptionsPresenter.vacancySortComparator());


    // ProgressDialog
    QObject::connect(&progressDialog.stopButton(), &QPushButton::released, &vacancySearchWorkflow, &VacancySearchWorkflow::interrupt);

    // VacancySearchWorkflow
    QObject::connect(&vacancySearchWorkflow, &VacancySearchWorkflow::vacancyCountDetermined, &progressDialog, &ProgressDialog::updateVacancyCount);
    QObject::connect(&vacancySearchWorkflow, &VacancySearchWorkflow::summaryReady, &progressDialog, &ProgressDialog::updateSummary);
    QObject::connect(&vacancySearchWorkflow, &VacancySearchWorkflow::detailsReady, &progressDialog, &ProgressDialog::updateDetails);
    QObject::connect(&vacancySearchWorkflow, &VacancySearchWorkflow::vacanciesReady, &vacancyBrowserPresenter, &VacancyBrowserPresenter::onVacanciesReady);
    QObject::connect(&vacancySearchWorkflow, &VacancySearchWorkflow::allActiveWorkFinished, &progressDialog, &ProgressDialog::accept);


    QObject::connect(this, &VacancyFinder::openProgressDialog, &progressDialog, &ProgressDialog::start);

    QObject::connect(this, &VacancyFinder::reset, &vacancyBrowserPresenter, &VacancyBrowserPresenter::reset);
    QObject::connect(this, &VacancyFinder::reset, &vacancySearchWorkflow, &VacancySearchWorkflow::reset);

    
    loadState();
}

VacancyFinder::~VacancyFinder()
{
    saveState();
    mainWindow.deleteLater();
}

void VacancyFinder::loadInterested()
{
    const auto& interestedVacancies = vacancyBrowserPresenter.interestedVacancies();
    if (interestedVacancies.isEmpty())
        return;

    const int maxVacancies = interestedVacancies.size();


    vacancySearchWorkflow.startSearch(interestedVacancies.values());
    emit openProgressDialog(maxVacancies);
}

void VacancyFinder::sendSearchRequest()
{
    const qsizetype maxVacancies = queryParametersPresenter.maxResults();

    const QString keywords = queryParametersPresenter.keywords();
    const QString location = queryParametersPresenter.location();

    qCInfo(VacancyFinderLog) << "Starting vacancy search; maximum results:" << maxVacancies;


    emit reset();
    vacancySearchWorkflow.startSearch(keywords, location, maxVacancies);
    emit openProgressDialog(maxVacancies);
}

void VacancyFinder::updateLoadInterestedButtonVisibility()
{
    queryParametersPresenter.setLoadInterestedButtonVisible(!vacancyBrowserPresenter.interestedVacancies().isEmpty());
}

void VacancyFinder::saveState()
{
    state.setIgnoredVacancies(vacancyBrowserPresenter.ignoredVacancies());
    state.setIgnoredCompanies(vacancyBrowserPresenter.ignoredCompanies());
    state.setInterestedVacancies(vacancyBrowserPresenter.interestedVacancies());


    state.setKeywords(queryParametersPresenter.keywords());
    state.setLocation(queryParametersPresenter.location());

    state.setMaxResults(queryParametersPresenter.maxResults());


    state.setFilterMode(resultViewOptionsPresenter.filterMode());
    state.setFilters(resultViewOptionsPresenter.filterWords());

    state.setSortMode(resultViewOptionsPresenter.sortMode());

    state.setIsUnreviewedVisible(resultViewOptionsPresenter.isUnreviewedVisible());
    state.setIsIgnoredVacanciesVisible(resultViewOptionsPresenter.isIgnoredVacanciesVisible());
    state.setIsIgnoredCompaniesVisible(resultViewOptionsPresenter.isIgnoredCompaniesVisible());
    state.setIsInterestedVisible(resultViewOptionsPresenter.isInterestedVisible());


    state.saveState();
}

void VacancyFinder::loadState()
{
    state.loadState();

    vacancyBrowserPresenter.setIgnoredVacancies(state.ignoredVacancies());
    vacancyBrowserPresenter.setIgnoredCompanies(state.ignoredCompanies());
    vacancyBrowserPresenter.setInterestedVacancies(state.interestedVacancies());


    queryParametersPresenter.setKeywords(state.keywords());
    queryParametersPresenter.setLocation(state.location());

    queryParametersPresenter.setMaxResults(state.maxResults());


    resultViewOptionsPresenter.setFilterMode(state.filterMode());
    resultViewOptionsPresenter.setFilterWords(state.filters());

    resultViewOptionsPresenter.setSortMode(state.sortMode());

    resultViewOptionsPresenter.setIsUnreviewedVisible(state.isUnreviewedVisible());
    resultViewOptionsPresenter.setIsIgnoredVacanciesVisible(state.isIgnoredVacanciesVisible());
    resultViewOptionsPresenter.setIsIgnoredCompaniesVisible(state.isIgnoredCompaniesVisible());
    resultViewOptionsPresenter.setIsInterestedVisible(state.isInterestedVisible());
}
