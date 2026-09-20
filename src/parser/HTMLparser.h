#pragma once
#include <QObject>

#include <QLoggingCategory>

#include "../model/Vacancy.h"


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
    void briefReady(Vacancy);
    void detailsReady(Vacancy);

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

    void parseBrief(const QString& html);


    void parseDetails(const QString& html, const QUrl& url);

    Vacancy parseDetailsByJson(const QJsonObject& object, const QUrl& url);
    Vacancy parseDetailsByHtml(const QString& html, const QUrl& url);

private:
    size_t maxVacancies;
    size_t totalVacancies;
    size_t emptyPagesAmount;

    QSet<QUrl> processedBriefs;
    QSet<QUrl> processedDetails;

    bool isInterrupted;
};

void saveToFile(const QString& html, const QString& name, const QString& folder);