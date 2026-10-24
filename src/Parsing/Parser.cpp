#include "Parser.h"

#include <AbstractParserStrategy.h>


Q_LOGGING_CATEGORY(ParserLog, "parsing.parser");


Parser::Parser(AbstractParserStrategy* const strategy, QObject* parent) :
    QObject(parent),
    m_strategy(strategy),
    m_processedBriefs(),
    m_processedDetails(),
    m_isInterrupted(false)
{
    Q_ASSERT_X(strategy, __FUNCTION__, "Strategy is nullptr");
    Q_ASSERT_X(strategy->thread() == thread(), __FUNCTION__, "Strategy must belong to the same thread as Parser");

    m_strategy->setParent(this);
}

const AbstractParserStrategy* const Parser::strategy() const
{
    return m_strategy;
}

void Parser::reset()
{
    m_processedBriefPages.clear();
    m_processedBriefs.clear();
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

    qCInfo(ParserLog) << url;

    const PageType pageType = m_strategy->classifyPage(html);
    switch (pageType)
    {
        case PageType::Brief:
        {
            processBriefs(html, url);
            break;
        }
        case PageType::Details:
        {
            processDetails(html, url);
            break;
        }
        default:
        {
            qCDebug(ParserLog) << "unknown PageType" << url;
            break;
        }
    }

    emit pageParsingFinished();
}

void Parser::interrupt()
{
    m_isInterrupted = true;

    qCDebug(ParserLog) << "Interrupted";
}

void Parser::processBriefs(const QString& html, const QUrl& url)
{
    if (m_processedBriefPages.contains(url))
    {
        qCDebug(ParserLog) << "Already processed page:" << url;
        return;
    }

    m_processedBriefPages.insert(url);

    QList<Vacancy> briefs = m_strategy->parseBriefs(html);
    for (Vacancy& brief : briefs)
    {
        if (m_processedBriefs.contains(brief.url))
        {
            qCDebug(ParserLog) << "Already processed brief:" << brief.url;
            continue;
        }
        m_processedBriefs.insert(brief.url);

        brief.id = m_processedBriefs.size();

        emit requestDetails(brief.url);
        emit briefParsed(brief);
    }
}

void Parser::processDetails(const QString& html, const QUrl& url)
{
    if (m_processedDetails.contains(url))
    {
        qCDebug(ParserLog) << "Already processed details:" << url;
        return;
    }

    m_processedDetails.insert(url);

    Vacancy details = m_strategy->parseDetails(html, url);
    details.id = m_processedDetails.size();

    emit detailsParsed(details);
}
