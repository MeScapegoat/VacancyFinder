#pragma once
// std

// Qt
#include <QThread>
#include <QLoggingCategory>
#include <QTime>

// custom
#include "Vacancy.h"
#include "NetworkManager.h"
#include "HTMLparser.h"
#include "GUI.h"

namespace Linked
{
    Q_DECLARE_LOGGING_CATEGORY(LinkedApp);
    class App : public QObject
    {
        Q_OBJECT

    public:
        App();
        ~App();

    public:
        QString loadTitleFromFile(const QString &path);
        QString loadDescriptionFromFile(const QString &path);

        const QList<Vacancy> &getVacancies();
        const QList<const Vacancy *> &getVacanciestoShow();

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
        void toList(Vacancy vacancy);

        void openSearchMessage();
        void closeSearchMessage();

    private:
        NetworkManager networkManager;
        QThread networkThread;

        HTMLparser parser;
        QThread parserThread;

        GUI gui;
        QThread guiThread;

        QList<Vacancy> vacancies;
        QList<const Vacancy *> vacanciesToShow;

        qsizetype descrProcessed = 0;

        QString baseUrl = "https://www.linkedin.com/jobs-guest/jobs/api/seeMoreJobPostings/search";
        QString keywords = "";
        QString location = "";

        QTime searchStartTime;
    };

} // namespace Linked