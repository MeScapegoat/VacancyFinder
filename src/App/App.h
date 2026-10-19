#pragma once

#include <QObject>

#include <QLoggingCategory>

#include "AppState.h"
#include <Vacancy.h>

class AppController;
class RequestManager;
class Parser;
class QueryParametersPresenter;
class ResultViewOptionsPresenter;
class VacancyBrowserPresenter;
class ProgressDialog;
class MainWindow;
class QNetworkReply;
class AbstractParserStrategy;
class AbstractRequestStrategy;

Q_DECLARE_LOGGING_CATEGORY(AppLog);

class App : public QObject
{
    Q_OBJECT

public:
    App(QObject* parent = nullptr);
    ~App();

public slots:
    void reset();
    void loadInterested();

    void sendSearchRequest();

    void updateMaxVacancies(const int maxVacancies);

    void onMeaningfulRepliesReceived(int amount);

signals:
    void initSearch(const QVector<QUrl>& urls);

    void openProgressDialog(const unsigned& maxVacancies);

protected:
    void saveState();
    void loadState();

private:
    QueryParametersPresenter& queryParametersPresenter;
    ResultViewOptionsPresenter& resultViewOptionsPresenter;
    VacancyBrowserPresenter& vacancyBrowserPresenter;

private:
    AppController& controller;

    MainWindow& mainWindow;
    ProgressDialog& progressDialog;

    AbstractRequestStrategy* requestStrategy;
    RequestManager& requestManager;
    QThread& networkThread;

    AbstractParserStrategy* parserStrategy;
    Parser& parser;
    QThread& parserThread;

    AppState state;

    QString keywords;
    QString location;

    unsigned maxVacancies;

    QTime searchStartTime;
};
