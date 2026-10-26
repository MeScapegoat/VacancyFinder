#pragma once

#include <QLoggingCategory>
#include <QObject>
#include <QSet>
#include <QString>
#include <QUrl>

#include <Vacancy.h>


Q_DECLARE_LOGGING_CATEGORY(ParserLog);


class AbstractParserStrategy;

class Parser : public QObject
{
    Q_OBJECT

public:
    // Takes ownership of strategy.
    Parser(AbstractParserStrategy& strategy, QObject* parent = nullptr);
    ~Parser() = default;

public:
    const AbstractParserStrategy& strategy() const;

public slots:
    virtual void reset();

    virtual void processPage(const QString& html, const QUrl& url);

    virtual void interrupt();

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
    AbstractParserStrategy& m_strategy;

    QSet<QUrl> m_processedBriefPages;

    QSet<QUrl> m_processedBriefs;
    QSet<QUrl> m_processedDetails;

    bool m_isInterrupted;
};
