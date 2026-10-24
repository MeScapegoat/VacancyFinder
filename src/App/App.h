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

Q_DECLARE_LOGGING_CATEGORY(AppLog);

class App : public QObject
{
    Q_OBJECT

public:
    App(QObject* parent = nullptr);
    ~App();

public slots:
    void loadInterested();

    void sendSearchRequest();

signals:
    void reset();

    void openProgressDialog(qsizetype maxVacancies);

protected:
    void saveState();
    void loadState();

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
