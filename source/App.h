#pragma once
// std

// Qt
#include <QThread>
#include <QLoggingCategory>

// custom
#include "Vacancy.h"
#include "NetworkManager.h"
#include "HTMLparser.h"

Q_DECLARE_LOGGING_CATEGORY(LinkedApp);

namespace Linked
{
    class App : public QObject
    {
        Q_OBJECT

    public:
        App();
        ~App() = default;

    public:
        void sendSearchRequest(QString, QString);

        QString loadTitleFromFile(const QString &path);
        QString loadDescriptionFromFile(const QString &path);

        const QList<Vacancy> &getVacancies();

        const QList<Vacancy> &sortByTime();
        const QList<Vacancy> &sortByTitle();
        const QList<Vacancy> &sortByCompany();
        const QList<Vacancy> &sortByLevel();
        const QList<Vacancy> &sortByRelevance(const QString &);

    public slots:
        void titleReady(Vacancy);
        void descrReady(const Vacancy&);

        void titlesProcessed();
        void requestFinished();
    
    signals:
        void sendRequest(QUrl url);

        // private:
    public:
        NetworkManager networkManager;
        QThread networkThread;

        HTMLparser parser;
        QThread parserThread;

        //Gui gui;
        //QThread guiThread;

        QList<Vacancy> vacancies;

        qsizetype descrProcessed = 0;

        QString baseUrl = "https://www.linkedin.com/jobs-guest/jobs/api/seeMoreJobPostings/search";
        QString keyword = "";
        QString location = "";
        //                     https://www.linkedin.com/jobs-guest/jobs/api/seeMoreJobPostings/search?keyword=C%2B%2B&location=Serbia&start=0
        //QString titleLink = "https://rs.linkedin.com/jobs-guest/jobs/api/seeMoreJobPostings/c%2B%2B-developer-jobs?countryRedirected=1&position=1&pageNum=0&start=%1";
    };

} // namespace Linked