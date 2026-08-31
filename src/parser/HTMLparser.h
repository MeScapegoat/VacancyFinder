#pragma once
#include <QObject>

#include <QLoggingCategory>

#include "../model/VacancyBrief.h"
#include "../model/VacancyDetails.h"


Q_DECLARE_LOGGING_CATEGORY(ParserLog);


class QJsonObject;

class HTMLparser : public QObject
{
    Q_OBJECT

public:
    HTMLparser(QObject* parent = nullptr);
    ~HTMLparser() = default;

public:
    void setMaxVacancies(const unsigned& newMaxVacancies);

    void reset();

public slots:
    void processHTML(const QString& html, const QUrl& url);

    void onInterrupt();

    void onRequestsFinished();

signals:
    void briefReady(VacancyBrief);
    void detailsReady(VacancyDetails);

    void sendRequest(const QUrl& ulr);

    void briefsFinished();
    void allDetailsFinished();

private:
    QString removeSpaces(QString str);
    QString extract(const QString& html, const QString& pattern);
    QStringList extractAll(const QString& html, const QString& pattern);

    static QString decodeHtml(QString text);

    void processBriefs(const QString& html, const int id);
    void processDetails(const QString& html, const QUrl& url);

    // fills field of Vacancy:
    // id
    // title
    // companyName
    // companyLink
    // location
    // date
    void parseBrief(const QString& html);


    void parseDetails(const QString& html, const QUrl& url);

    // fills field of Vacancy:
    // industry
    // expirationDate
    // experience
    // education
    // skills
    // employmentType
    // description
    VacancyDetails parseDetailsByJson(const QJsonObject& object, const QUrl& url);

    // fills field of Vacancy:
    // industry
    // level
    // employmentType
    // description
    VacancyDetails parseDetailsByHtml(const QString& html, const QUrl& url);

private:
    unsigned maxVacancies;
    unsigned totalVacancies;
    unsigned emptyPagesAmount;

    QSet<QUrl> processedBriefs;
    QSet<QUrl> processedDetails;

    bool isInterrupted;
};

void saveToFile(const QString& html, const QString& name, const QString& folder);