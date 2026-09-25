#pragma once
// std

// Qt
#include <QThread>
#include <QLoggingCategory>
#include <QTime>

// custom
#include <Vacancy.h>
#include <HTMLparser.h>
#include <AppState.h>
#include <MainWindow.h>
#include <ResultViewOptionsEnums.h>


class RequestManager;
class HTMLparser;

class QueryParametersPresenter;
class ResultViewOptionsPresenter;

Q_DECLARE_LOGGING_CATEGORY(AppLog);

class ProgressDialog;
class Vacancy;

// TODO: current App workflow is a mess, need to create AppController or smth

class App : public QObject
{
    Q_OBJECT

public:
    App(QObject* parent = nullptr);
    ~App();

public:
    using constVacancyRef = std::reference_wrapper<const Vacancy>;

public:
    QHash<QUrl, Vacancy>& getVacancies();
    const QHash<QUrl, Vacancy>& getVacancies() const;
    const QList<const Vacancy*>& getVacanciestoShow() const;
    const unsigned& getMaxVacancies() const;

public:
    QString loadTitleFromFile(const QString& path);
    QString loadDescriptionFromFile(const QString& path);

public slots:
    void reset();
    void loadInterested();

    void sendSearchRequest();
    void openVacancyDescription(const QModelIndex&);
    void closeVacancyDescription();

    void updateDescriptionList();

    void updateMaxVacancies(const int maxVacancies);

    void onBriefReady(const Vacancy& brief);
    void onDetailsReady(const Vacancy& details);

    void onBriefsFinished();
    void onAllDetailsFinished();
    void onRequestsFinished();

    void onIgnoreButtonPressed();
    void onInterestButtonPressed();
    void onRemoveIgnoreButtonPressed();
    void onRemoveInterestButtonPressed();

signals:
    void initSearch(const QVector<QUrl>& urls);
    void updateVacanciesList(const QStringList& data);

    void openProgressDialog(const unsigned& maxVacancies);
    void closeProgressDialog();

protected:
    void syncVacanciesToShow();

    void updateIsAllFinished();

    void applyStatusFilter();
    void applyFiltering();
    void applySorting();
    void updateModelData();


    void sortByRelevance();
    void sortByTitle();
    void sortByCompany();
    void sortByTime();
    void sortByEmploymentType();
    void sortByLevel();
    void sortByExperience();
    void sortByEducation();
    void sortBySkills();
    void sortByIndustry();
    void sortByExpirationDate();

    std::optional<constVacancyRef> getCurrentVacancy() const;

    VacancyStatus getVacancyStatus(const Vacancy& vacancy);

protected:
    void saveState();
    void loadState();

private:
    QueryParametersPresenter& queryParametersPresenter;
    ResultViewOptionsPresenter& resultViewOptionsPresenter;

private:
    MainWindow& mainWindow;
    ProgressDialog& progressDialog;

    RequestManager& requestManager;
    QThread* networkThread = nullptr;   // TODO: why is it pointers?

    HTMLparser* parser = nullptr;       // TODO: why is it pointers?
    QThread* parserThread = nullptr;    // TODO: why is it pointers?

    AppState state;

    QHash<QUrl, Vacancy> vacancies;
    QVector<const Vacancy*> vacanciesToShow;

    QSet<QUrl> ignoredVacancies;
    QSet<QUrl> interestedVacancies;

    QString keywords;
    QString location;

    unsigned maxVacancies;

    QTime searchStartTime;

    bool isRequestsFinished;
    bool isBriefsFinished;
    bool isAllDetailsFinished;

    bool isAllFinished;
};