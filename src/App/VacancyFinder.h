#pragma once

#include <QLoggingCategory>
#include <QObject>
#include <QString>

#include "AppState.h"

class VacancySearchWorkflow;
class QueryParametersPresenter;
class ResultViewOptionsPresenter;
class VacancyBrowserPresenter;
class ProgressDialog;
class MainWindow;

Q_DECLARE_LOGGING_CATEGORY(VacancyFinderLog);

class VacancyFinder : public QObject
{
    Q_OBJECT

public:
    VacancyFinder(QObject* parent = nullptr);
    ~VacancyFinder();

public slots:
    void loadInterested();

    void sendSearchRequest();

signals:
    void reset();

    void openProgressDialog(qsizetype maxVacancies);

protected:
    void saveState();
    void loadState();

private slots:
    void updateLoadInterestedButtonVisibility();

private:
    QueryParametersPresenter& queryParametersPresenter;
    ResultViewOptionsPresenter& resultViewOptionsPresenter;
    VacancyBrowserPresenter& vacancyBrowserPresenter;

private:
    VacancySearchWorkflow& vacancySearchWorkflow;

    MainWindow& mainWindow;
    ProgressDialog& progressDialog;

    AppState state;
};
