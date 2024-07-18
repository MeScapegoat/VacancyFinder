#pragma once
// std

// Qt
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QNetworkReply>
#include <QSet>

// custom
#include "LinkedVacancy.h"

class LinkedApp : public QObject
{
    Q_OBJECT

public:
    LinkedApp(QObject *parent = nullptr);
    ~LinkedApp() = default;

public:
    void sendSearchRequest(QString req = "C++", QString location = "Serbia");
    void sendRequest(QString link);

    QString loadTitleFromFile(const QString &path);
    QString loadDescriptionFromFile(const QString &path);

    const QList<LinkedVacancy> &getVacancies();

    const QList<LinkedVacancy> &sortByTime();
    const QList<LinkedVacancy> &sortByTitle();
    const QList<LinkedVacancy> &sortByCompany();
    const QList<LinkedVacancy> &sortByLevel();
    const QList<LinkedVacancy> &sortByRelevance(const QString &);

public slots:
    void processHTML(QNetworkReply *reply);

    // private:
public:
    void processTitle(QNetworkReply *reply);
    void processDescription(QNetworkReply *reply);

    QString parseTitle(QString);
    QString parseDescription(QString);

    void finishedRequest();

public:
    static void removeSpaces(QString &);

private:
    enum class RequestType
    {
        title,
        description
    };

    // private:
public:
    QNetworkAccessManager networkManager;
    QNetworkRequest request;
    RequestType currrentRequestType;

    QList<LinkedVacancy> vacancies; // no description for now
    LinkedVacancy currentVacancy;   // proccesing right now

    qsizetype descriptionsFinished = 0;

    QString descriptionLink;
    QString titleLink = "https://rs.linkedin.com/jobs-guest/jobs/api/seeMoreJobPostings/c%2B%2B-developer-jobs?countryRedirected=1&position=1&pageNum=0&start=%1";
};