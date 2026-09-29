#pragma once

#include <QObject>

#include <QLoggingCategory>

#include "AppState.h"
#include <ResultViewOptionsEnums.h>
#include <Vacancy.h>

class AppController;
class RequestManager;
class HTMLparser;
class QueryParametersPresenter;
class ResultViewOptionsPresenter;
class ProgressDialog;
class MainWindow;
class HTMLparser;
class QNetworkReply;

Q_DECLARE_LOGGING_CATEGORY(AppLog);

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
    // TODO: bad class for this
    QString loadTitleFromFile(const QString& path);
    QString loadDescriptionFromFile(const QString& path);

public slots:
    void reset();
    void loadInterested();

    void sendSearchRequest();

    // TODO: bad class for this
    void openVacancyDescription(const QModelIndex&);
    void closeVacancyDescription();

    void updateDescriptionList();

    void updateMaxVacancies(const int maxVacancies);

    void onBriefReady(const Vacancy& brief);
    void onDetailsReady(const Vacancy& details);

    void onIgnoreButtonPressed();
    void onInterestButtonPressed();
    void onRemoveIgnoreButtonPressed();
    void onRemoveInterestButtonPressed();

    void onMeaningfulRepliesReceived(int amount);

signals:
    void initSearch(const QVector<QUrl>& urls);
    void updateVacanciesList(const QStringList& data);

    void openProgressDialog(const unsigned& maxVacancies);

protected:
    void syncVacanciesToShow();

    void applyStatusFilter();
    void applyFiltering();
    void applySorting();
    void updateModelData();

    // TODO: bad class for this
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
    AppController& controller;

    MainWindow& mainWindow;
    ProgressDialog& progressDialog;

    RequestManager& requestManager;
    QThread& networkThread;

    HTMLparser& parser;
    QThread& parserThread;

    AppState state;

    QHash<QUrl, Vacancy> vacancies;
    QVector<const Vacancy*> vacanciesToShow;

    QSet<QUrl> ignoredVacancies;
    QSet<QUrl> interestedVacancies;

    QString keywords;
    QString location;

    unsigned maxVacancies;

    QTime searchStartTime;
};