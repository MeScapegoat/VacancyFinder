#pragma once
// std

// Qt
#include <QThread>
#include <QLoggingCategory>

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
        void descrReady(const Vacancy &);

        void titlesProcessed();
        void requestFinished();

    signals:
        void sendRequest(QUrl url);

    private:
        NetworkManager networkManager;
        QThread networkThread;

        HTMLparser parser;
        QThread parserThread;

        GUI gui;
        QThread guiThread;

        QList<Vacancy> vacancies;

        qsizetype descrProcessed = 0;

        QString baseUrl = "https://www.linkedin.com/jobs-guest/jobs/api/seeMoreJobPostings/search";
        QString keyword = "";
        QString location = "";
    };

} // namespace Linked