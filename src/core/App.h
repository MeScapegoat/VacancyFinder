#pragma once
// std

// Qt
#include <QThread>
#include <QLoggingCategory>
#include <QTime>

// custom
#include "../model/Vacancy.h"
#include "../model/VacancyBrief.h"
#include "../model/VacancyDetails.h"
#include "SharedContext.h"
#include "../parser/HTMLparser.h"
#include "../gui/GUI.h"

class RequestManager;
class HTMLparser;

namespace Linked
{
    Q_DECLARE_LOGGING_CATEGORY(LinkedApp);

    class GUI;

    // TODO: current App workflow is a mess, need to create AppController or smth

    class App : public QObject
    {
        Q_OBJECT

    public:
        App(QObject* parent = nullptr);
        ~App();

    public:
        QHash<QUrl, Vacancy>& getVacancies();
        const QHash<QUrl, Vacancy>& getVacancies() const;
        const QList<const Vacancy*>& getVacanciestoShow() const;
        const unsigned& getMaxVacancies() const;

    public:
        QString loadTitleFromFile(const QString& path);
        QString loadDescriptionFromFile(const QString& path);

    public slots:
        void sendSearchRequest();
        void openVacancyDescription(const QModelIndex&);

        void updateDescriptionList(int temp = 0);

        void updateMaxVacancies(const unsigned newMaxVacancies);

        void onBriefReady(const VacancyBrief& brief);
        void onDetailsReady(const VacancyDetails& details);

        void onBriefsFinished();
        void onAllDetailsFinished();
        void onRequestsFinished();

    signals:
        void initSearch(const QVector<QUrl>& urls);
        void updateVacanciesList();

        void openProgressDialog(const unsigned& maxVacancies);
        void closeProgressDialog();

    protected:
        void syncVacanciesToShow();

        void updateIsAllFinished();

        void applyFiltering();
        void applySorting();


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
        

    private:
        SharedContext* context;

    private:
        RequestManager& requestManager;
        QThread* networkThread = nullptr;

        HTMLparser* parser = nullptr;
        QThread* parserThread = nullptr;

        GUI* gui = nullptr;

        QHash<QUrl, Vacancy> vacancies;
        QVector<const Vacancy*> vacanciesToShow;

        QString baseUrl = "https://www.linkedin.com/jobs-guest/jobs/api/seeMoreJobPostings/search";
        QString keywords = "";
        QString location = "";

        unsigned maxVacancies;

        QTime searchStartTime;

        bool isRequestsFinished;
        bool isBriefsFinished;
        bool isAllDetailsFinished;

        bool isAllFinished;
    };

} // namespace Linked