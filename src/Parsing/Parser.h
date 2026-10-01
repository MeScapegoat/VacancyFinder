#pragma once

#include <QObject>

#include <QLoggingCategory>

#include <Vacancy.h>


Q_DECLARE_LOGGING_CATEGORY(ParserLog);


class AbstractParserStrategy;

class Parser : public QObject
{
    Q_OBJECT

public:
    Parser(QObject* parent = nullptr);
    ~Parser() = default;

public:
    void setStrategy(const AbstractParserStrategy* const strategy);
    bool hasStrategy() const;

    void reset();

public slots:
    void processPage(const QString& html, const QUrl& url);

    void interrupt();

signals:
    void pageParsingStarted(int amount = 1);
    void pageParsingFinished(int amount = 1);

    void briefParsed(Vacancy); // TODO: pass by smart pointer?
    void detailsParsed(Vacancy); // TODO: pass by smart pointer?

    void requestDetails(const QUrl& url);

private:
    void processBriefs(const QString& html, const QUrl& url);
    void processDetails(const QString& html, const QUrl& url);

private:
    const AbstractParserStrategy* strategy;

    QSet<QUrl> m_processedBriefPages;

    QSet<QUrl> m_processedBriefs;
    QSet<QUrl> m_processedDetails;

    bool m_isInterrupted;
};