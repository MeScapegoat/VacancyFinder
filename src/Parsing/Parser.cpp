#include "Parser.h"

#include <AbstractParserStrategy.h>


Q_LOGGING_CATEGORY(ParserLog, "parsing.parser");


Parser::Parser(QObject* parent) :
    QObject(parent),
    strategy(nullptr),
    m_processedBriefs(),
    m_processedDetails(),
    m_isInterrupted(false)
{
}

void Parser::setStrategy(const AbstractParserStrategy* const newStrategy)
{
    Q_ASSERT(newStrategy);

    if (strategy == newStrategy)
        return;

    strategy = newStrategy;
}

bool Parser::hasStrategy() const
{
    return strategy !=  nullptr;
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

    const PageType pageType = strategy->classifyPage(html);
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

    QList<Vacancy> briefs = strategy->parseBriefs(html);
    for (Vacancy& brief : briefs)
    {
        if (m_processedBriefs.contains(brief.link))
        {
            qCDebug(ParserLog) << "Already processed brief:" << brief.link;
            continue;
        }
        m_processedBriefs.insert(brief.link);

        brief.id = m_processedBriefs.size();

        emit requestDetails(brief.link);
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

    Vacancy details = strategy->parseDetails(html, url);
    details.id = m_processedDetails.size();

    emit detailsParsed(details);
}