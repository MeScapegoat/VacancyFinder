#pragma once
// std

// Qt
#include <QThread>
#include <QLoggingCategory>
#include <QTime>

// custom
#include "Vacancy.h"
#include "SharedContext.h"
#include "../network/NetworkManager.h"
#include "../network/HTMLparser.h"
#include "../gui/GUI.h"

namespace Linked
{
    Q_DECLARE_LOGGING_CATEGORY(LinkedApp);

    class NetworkManager;
    class GUI;
    class HTMLparser;

    class App : public QObject
    {
        Q_OBJECT

    public:
        App();
        ~App();

    public:
        static QList<Vacancy> &getVacancies();
        static const QList<const Vacancy *> &getVacanciestoShow();

    public:
        QString loadTitleFromFile(const QString &path);
        QString loadDescriptionFromFile(const QString &path);

        void sortByTime();
        void sortByTitle();
        void sortByCompany();
        void sortByLevel();
        void sortByRelevance(const QString &);

    public slots:
        void titleReady(const Vacancy &);
        void descrReady(const Vacancy &);

        void sendSearchRequest();
        void openVacancyDescription(const QModelIndex &);

        void titlesProcessed();
        void requestFinished();

        void updateDescriptionList(int temp = 0);

    signals:
        void sendRequest(QUrl url);
        void updateVacanciesList();

        void openProgressDialog();
        void closeSearchMessage();

    private:
        SharedContext *context;

    private:
        NetworkManager *networkManager = nullptr;
        QThread *networkThread = nullptr;

        HTMLparser *parser = nullptr;
        QThread *parserThread = nullptr;

        GUI *gui = nullptr;

        static QList<Vacancy> vacancies;
        static QList<const Vacancy *> vacanciesToShow;

        qsizetype descrProcessed = 0;

        QString baseUrl = "https://www.linkedin.com/jobs-guest/jobs/api/seeMoreJobPostings/search";
        QString keywords = "";
        QString location = "";

        QTime searchStartTime;
        bool isRequestFinished = false;
    };

} // namespace Linked