#pragma once

#include <QHash>
#include <QList>
#include <QObject>
#include <QString>
#include <QThread>
#include <QUrl>


#include <Vacancy.h>

class AbstractSearchUrlProvider;
class Parser;
class RequestManager;
class WorkCoordinator;
class QThread;


class VacancySearchWorkflow : public QObject
{
    Q_OBJECT

public:
    // Takes ownership of all three dependencies.
    VacancySearchWorkflow(AbstractSearchUrlProvider& searchUrlProvider,
                          RequestManager& requestManager,
                          Parser& parser,
                          QObject* parent = nullptr);
    ~VacancySearchWorkflow() override;

public slots:
    void startSearch(const QList<QUrl> urls);
    void startSearch(const QString& keywords, const QString& location, qsizetype maxVacancies);

signals:
    void interrupt();
    void reset();

    void vacanciesReady(const QList<Vacancy>& vacancies);

    void vacancyCountDetermined(int count);
    void allActiveWorkFinished();

    void urlsRequested(const QList<QUrl>& urls);

    void briefReady(const Vacancy& vacancy);
    void detailsReady(const Vacancy& vacancy);

private slots:
    void onBriefParsed(const Vacancy& vacancy);
    void onDetailsParsed(const Vacancy& vacancy);

    void onWorkFinished();

    void onReset();

private:
    void storeVacancy(const Vacancy& vacancy);

private:
    AbstractSearchUrlProvider& m_searchUrlProvider;

    RequestManager& m_requestManager;
    Parser& m_parser;
    WorkCoordinator& m_workCoordinator;

    QThread& m_networkThread;
    QThread& m_parserThread;

    QHash<QUrl, Vacancy> m_vacancyByUrl;
};
