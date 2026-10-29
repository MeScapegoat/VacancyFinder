#include "Parser.h"

#include <QLoggingCategory>
#include <QUrl>

#include <AbstractParserStrategy.h>


Q_LOGGING_CATEGORY(ParserLog, "parsing.parser");


Parser::Parser(AbstractParserStrategy& strategy, QObject* parent) :
    QObject(parent),
    m_strategy(strategy),
    m_processedSummaries(),
    m_processedDetails(),
    m_isInterrupted(false)
{
    Q_ASSERT_X(strategy.thread() == thread(), __FUNCTION__, "Strategy must belong to the same thread as Parser");

    strategy.setParent(this);
}

const AbstractParserStrategy& Parser::strategy() const
{
    return m_strategy;
}

void Parser::reset()
{
    m_processedSummaryPages.clear();
    m_processedSummaries.clear();
    m_processedDetails.clear();

    m_isInterrupted = false;
}

void Parser::processPage(const QString& html, const QUrl& url)
{
    if (m_isInterrupted)
    {
        return;
    }

    emit pageParsingStarted();

    qCDebug(ParserLog) << "Parsing page:" << url;

    const PageType pageType = m_strategy.classifyPage(html);
    switch (pageType)
    {
        case PageType::Summary:
        {
            processSummaries(html, url);
            break;
        }
        case PageType::Details:
        {
            processDetails(html, url);
            break;
        }
        default:
        {
            qCWarning(ParserLog) << "Could not classify page:" << url;
            break;
        }
    }

    emit pageParsingFinished();
}

void Parser::interrupt()
{
    m_isInterrupted = true;
}

void Parser::summaryRequestsFinished()
{
    emit vacancyCountDetermined(m_processedSummaries.size());
}

void Parser::processSummaries(const QString& html, const QUrl& url)
{
    if (m_processedSummaryPages.contains(url))
    {
        return;
    }

    m_processedSummaryPages.insert(url);

    QList<Vacancy> summaries = m_strategy.parseSummaries(html);
    for (Vacancy& summary : summaries)
    {
        if (m_processedSummaries.contains(summary.url))
        {
            continue;
        }
        m_processedSummaries.insert(summary.url);

        summary.id = m_processedSummaries.size();

        emit requestDetails(summary.url);
        emit summaryParsed(summary);
    }
}

void Parser::processDetails(const QString& html, const QUrl& url)
{
    if (m_processedDetails.contains(url))
    {
        return;
    }

    m_processedDetails.insert(url);

    Vacancy details = m_strategy.parseDetails(html, url);
    details.id = m_processedDetails.size();

    emit detailsParsed(details);
}
