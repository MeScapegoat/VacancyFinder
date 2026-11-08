#include <atomic>

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QElapsedTimer>
#include <QList>
#include <QString>
#include <QTimer>

#include "BenchmarkCommonData.h"
#include "BenchmarkLinkedInRequestStrategy.h"
#include <LinkedInParserStrategy/LinkedInParserStrategy.h>
#include <LinkedInSearchUrlProvider.h>
#include <NetworkManager.h>
#include <Parser.h>
#include <RequestManager.h>
#include <Vacancy.h>
#include <VacancySearchWorkflow.h>

static constexpr qsizetype WaitBetweenTestsMs = 60 * 1000;
static constexpr qsizetype MaximumVacancies = 1000;
static constexpr qsizetype TotalRuns = 1;
static const QString SearchKeywords = QStringLiteral("2d concept artists");
static const QString SearchLocation = QStringLiteral("");



class BenchmarkRunner : public QObject
{
public:
    explicit BenchmarkRunner(QObject* parent = nullptr) :
        QObject(parent)
    {
        configurations =
        {
            {4, 10, 5000},
            {4, 11, 5000},
            {4, 12, 5000},
            {4, 14, 5000},
            {5, 10, 5000},
            {6, 10, 5000},
            {4, 10, 4500},
            {4, 10, 4000},
        };
    }

    void start()
    {
        runNext();
    }

    void setFileName(const QString& _fileName)
    {
        fileName = _fileName;
    }

private:
    void runNext()
    {
        if (configurations.isEmpty() || scheduleIndex >= configurations.size() * TotalRuns)
        {
            printResults(results, SortColumn::ElapsedMs);
            QCoreApplication::quit();
            return;
        }

        const qsizetype configurationCount = configurations.size();
        const qsizetype runRoundIndex = scheduleIndex / configurationCount;
        const qsizetype positionInRound = scheduleIndex % configurationCount;
        // Rotate each round to spread configurations across different positions.
        const qsizetype roundOffset = runRoundIndex * configurationCount / TotalRuns;

        currentConfigurationIndex = (positionInRound + roundOffset) % configurationCount;
        currentRunNumber = runRoundIndex + 1;

        const BenchmarkConfig config = configurations[currentConfigurationIndex];

        requestAttempts.store(0);
        meaningfulReplies.store(0);
        meaninglessReplies.store(0);
        summariesParsed.store(0);
        detailsParsed.store(0);
        vacanciesCollected = 0;

        qInfo() << "";
        qInfo() << "========================================";
        qInfo() << "Starting benchmark";
        qInfo() << "Run:               " << currentRunNumber << "/" << TotalRuns;
        qInfo() << "MaxConcurrent:     " << config.maxConcurrent;
        qInfo() << "RequestsPerSecond: " << config.requestsPerSecond;
        qInfo() << "RequestCooldownMs: " << config.requestCooldownMs;
        qInfo() << "Maximum vacancies: " << MaximumVacancies;
        qInfo() << "Keywords:          " << SearchKeywords;
        qInfo() << "Location:          " << SearchLocation;
        qInfo() << "========================================";

        LinkedInSearchUrlProvider* searchUrlProvider = new LinkedInSearchUrlProvider;
        NetworkManager* networkManager = new NetworkManager;
        requestStrategy = new BenchmarkLinkedInRequestStrategy(
            networkManager,
            config.requestsPerSecond,
            config.requestCooldownMs,
            config.maxConcurrent
        );
        RequestManager* requestManager = new RequestManager(*requestStrategy);
        LinkedInParserStrategy* parserStrategy = new LinkedInParserStrategy;
        Parser* parser = new Parser(*parserStrategy);

        QObject::connect(requestManager, &RequestManager::requestsStarted, this, [this](int amount)
        {
            requestAttempts.fetch_add(amount);
        }, Qt::DirectConnection);

        QObject::connect(requestManager, &RequestManager::meaningfulRepliesReceived, this, [this](int amount)
        {
            meaningfulReplies.fetch_add(amount);
        }, Qt::DirectConnection);

        QObject::connect(requestManager, &RequestManager::meaninglessRepliesReceived, this, [this](int amount)
        {
            meaninglessReplies.fetch_add(amount);
        }, Qt::DirectConnection);

        QObject::connect(parser, &Parser::summaryParsed, this, [this](const Vacancy&)
        {
            summariesParsed.fetch_add(1);
        }, Qt::DirectConnection);

        QObject::connect(parser, &Parser::detailsParsed, this, [this](const Vacancy&)
        {
            detailsParsed.fetch_add(1);
        }, Qt::DirectConnection);

        workflow = new VacancySearchWorkflow(*searchUrlProvider, *requestManager, *parser, this);

        QObject::connect(workflow, &VacancySearchWorkflow::vacanciesReady, this, [this](const QList<Vacancy>& vacancies)
        {
            vacanciesCollected = vacancies.size();
        });

        connect(workflow, &VacancySearchWorkflow::allActiveWorkFinished, this, [this]()
        {
            finishCurrentBenchmark();
        }, Qt::QueuedConnection);

        timer.start();
        workflow->startSearch(SearchKeywords, SearchLocation, MaximumVacancies);
    }

    void finishCurrentBenchmark()
    {
        const qint64 elapsedMs = timer.elapsed();
        const BenchmarkConfig config = configurations[currentConfigurationIndex];
        const qint64 requestAttemptsValue = requestAttempts.load();
        const qint64 meaningfulRepliesValue = meaningfulReplies.load();
        const qint64 meaninglessRepliesValue = meaninglessReplies.load();
        const qint64 unmatchedAttemptsValue = requestAttemptsValue - meaningfulRepliesValue - meaninglessRepliesValue;

        qInfo() << "Finished run:";
        qInfo() << "  Run:               " << currentRunNumber << "/" << TotalRuns;
        qInfo() << "  MaxConcurrent:     " << config.maxConcurrent;
        qInfo() << "  RequestsPerSecond: " << config.requestsPerSecond;
        qInfo() << "  RequestCooldownMs: " << config.requestCooldownMs;
        qInfo() << "  Elapsed:           " << elapsedMs / 1000 << "secs";
        qInfo() << "  Request attempts:  " << requestAttemptsValue;
        qInfo() << "  Useful replies:    " << meaningfulRepliesValue;
        qInfo() << "  Non-useful replies:" << meaninglessRepliesValue;
        qInfo() << "  Unmatched attempts:" << unmatchedAttemptsValue;
        const qint64 summariesParsedValue = summariesParsed.load();
        const qint64 detailsParsedValue = detailsParsed.load();

        qInfo() << "  Summaries parsed:  " << summariesParsedValue;
        qInfo() << "  Details parsed:    " << detailsParsedValue;
        qInfo() << "  Vacancies collected:" << vacanciesCollected;
        const double vacanciesPerMinute = elapsedMs > 0
            ? static_cast<double>(vacanciesCollected) * 60'000 / elapsedMs
            : 0.0;
        qInfo() << "  Vacancies/min:     " << vacanciesPerMinute;

        const BenchmarkRequestStatistics requestStatistics = requestStrategy->statistics();
        qInfo() << "  HTTP successful:  " << requestStatistics.successfulResponses;
        qInfo() << "  HTTP 429:         " << requestStatistics.rateLimitedResponses;
        qInfo() << "  429 with Retry-After:" << requestStatistics.rateLimitedResponsesWithRetryAfter;
        qInfo() << "  429 without Retry-After:" << requestStatistics.rateLimitedResponsesWithoutRetryAfter;
        qInfo() << "  Retry-After values/counts:" << requestStatistics.retryAfterHeaderCounts;
        qInfo() << "  Other failures:   " << requestStatistics.failedResponses;
        qInfo() << "  Empty summary:    " << requestStatistics.emptySummaryResponses;
        qInfo() << "  Cooldown time:    " << requestStatistics.totalCooldownTimeMs << "ms";

        const BenchmarkRun currentRun{
            currentRunNumber,
            elapsedMs,
            requestAttemptsValue,
            meaningfulRepliesValue,
            meaninglessRepliesValue,
            unmatchedAttemptsValue,
            summariesParsedValue,
            detailsParsedValue,
            vacanciesCollected,
            requestStatistics.successfulResponses,
            requestStatistics.rateLimitedResponses,
            requestStatistics.rateLimitedResponsesWithRetryAfter,
            requestStatistics.rateLimitedResponsesWithoutRetryAfter,
            requestStatistics.retryAfterHeaderCounts,
            requestStatistics.failedResponses,
            requestStatistics.emptySummaryResponses,
            requestStatistics.totalCooldownTimeMs,
            MaximumVacancies,
            SearchKeywords,
            SearchLocation
        };

        qsizetype resultIndex = 0;
        while (resultIndex < results.size() && !(results[resultIndex].config == config))
            ++resultIndex;

        if (resultIndex == results.size())
            results.append({config, {}});

        results[resultIndex].runs.append(currentRun);

        if (!saveResults(results, fileName))
            qWarning() << "Saving the current run failed; see the save error above.";


        workflow->deleteLater();
        workflow = nullptr;
        requestStrategy = nullptr;

        ++scheduleIndex;
        if (scheduleIndex >= configurations.size() * TotalRuns)
            runNext();
        else
            QTimer::singleShot(WaitBetweenTestsMs, this, &BenchmarkRunner::runNext);
    }

private:
    VacancySearchWorkflow* workflow = nullptr;
    BenchmarkLinkedInRequestStrategy* requestStrategy = nullptr;

    QList<BenchmarkConfig> configurations;
    QList<BenchmarkResult> results;

    
    QElapsedTimer timer;
    qsizetype scheduleIndex = 0;
    qsizetype currentConfigurationIndex = 0;
    qsizetype currentRunNumber = 0;

    QString fileName;
    std::atomic<qint64> requestAttempts = 0;
    std::atomic<qint64> meaningfulReplies = 0;
    std::atomic<qint64> meaninglessReplies = 0;
    std::atomic<qint64> summariesParsed = 0;
    std::atomic<qint64> detailsParsed = 0;
    qint64 vacanciesCollected = 0;
};

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    BenchmarkRunner runner;

    const QString fileName = QString("benchmark_%1.json").arg(QDateTime::currentDateTime().toString("dd-MM-yyyy_HH-mm-ss"));

    runner.setFileName(fileName);

    runner.start();

    return app.exec();
}
