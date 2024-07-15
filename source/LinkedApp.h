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
    void sendRequest();
    void getVacancies(); // TODO return vector of vacancies

public slots:
    void parseHTML(QNetworkReply *reply); // TODO temporrary

private:
    void removeSpaces(QString &);
    // consts
private:
    const QString link = "https://rs.linkedin.com/jobs-guest/jobs/api/seeMoreJobPostings/c%2B%2B-developer-jobs?countryRedirected=1&position=1&pageNum=0&start=%1";

private:
    QNetworkAccessManager networkManager;
    QNetworkRequest request;
    QList<LinkedVacancy> vacancies; // only names for now
    QSet<QString> links;
    QSet<QString> names;
    bool vacanciesProcessed = false;
};