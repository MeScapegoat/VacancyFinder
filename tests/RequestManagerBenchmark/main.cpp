#include <QCoreApplication>
#include <QElapsedTimer>
#include <QList>
#include <QTimer>
#include <QDebug>

#include "network/RequestManager.h"

static constexpr unsigned waitBetweenTestsMs = 120 * 1000;
static constexpr unsigned linksAmount = 100;

struct BenchmarkConfig
{
    unsigned maxConcurrent;
    unsigned requestsPerSecond;
    unsigned requestCooldownMs;
};

struct BenchmarkResult
{
    BenchmarkConfig config;
    qint64 elapsedMs;
    quint64 errorCount;
};

class BenchmarkRunner : public QObject
{
public:
    explicit BenchmarkRunner(QObject* parent = nullptr) :
        QObject(parent)
    {
        configurations =
        {
            {1, 1, 0},
            {8, 1, 0},
            {1, 8, 0},
            
            {2, 2, 0},
            {4, 4, 0},
            {8, 8, 0},
            {16, 16, 0},

            {2, 2, 2000},
            {4, 4, 2000},
            {8, 8, 2000},
            {16, 16, 2000},

            {2, 2, 4000},
            {4, 4, 4000},
            {8, 8, 4000},
            {16, 16, 4000},

            {2, 2, 8000},
            {4, 4, 8000},
            {8, 8, 8000},
            {16, 16, 8000},

            {2, 2, 16000},
            {4, 4, 16000},
            {8, 8, 16000},
            {16, 16, 16000},
        };
    }

    void start()
    {
        runNext();
    }

private:
    void runNext()
    {
        if (configurationIndex >= configurations.size())
        {
            printResults();
            QCoreApplication::quit();
            return;
        }

        const BenchmarkConfig config = configurations[configurationIndex];

        qInfo() << "";
        qInfo() << "========================================";
        qInfo() << "Starting benchmark";
        qInfo() << "MaxConcurrent:" << config.maxConcurrent;
        qInfo() << "RequestsPerSecond:" << config.requestsPerSecond;
        qInfo() << "RequestCooldownMs:" << config.requestCooldownMs;
        qInfo() << "========================================";

        requestManager = new RequestManager(this);

        requestManager->setMaxConcurrent(config.maxConcurrent);
        requestManager->setRequestsPerSecond(config.requestsPerSecond);
        requestManager->setRequestCooldownMs(config.requestCooldownMs);

        connect(requestManager, &RequestManager::requestsFinished, this, [this]()
        {
            finishCurrentBenchmark();
        });

        timer.start();

        static QList<QUrl> urls = testUrls();

        requestManager->addUrls(urls);
    }

    void finishCurrentBenchmark()
    {
        const qint64 elapsedMs = timer.elapsed();
        const quint64 errorCount = requestManager->getErrorCount();
        const BenchmarkConfig config = configurations[configurationIndex];

        qInfo() << "Finished:";
        qInfo() << "  MaxConcurrent:     " << config.maxConcurrent;
        qInfo() << "  RequestsPerSecond: " << config.requestsPerSecond;
        qInfo() << "  RequestCooldownMs: " << config.requestCooldownMs;
        qInfo() << "  errorCount:        " << errorCount;
        qInfo() << "  Elapsed:           " << elapsedMs << "ms";
        

        results.append({config, elapsedMs, errorCount});

        requestManager->deleteLater();
        requestManager = nullptr;

        ++configurationIndex;

        QTimer::singleShot(waitBetweenTestsMs, this, &BenchmarkRunner::runNext);
    }

    void printResults()
    {
        qInfo() << "";
        qInfo() << "========================================";
        qInfo() << "BENCHMARK RESULTS";
        qInfo() << "========================================";

        for (const BenchmarkResult& result : results)
        {
            qInfo()
                << "MaxConcurrent: " << result.config.maxConcurrent
                << "RPS:           " << result.config.requestsPerSecond
                << "Cooldown:      " << result.config.requestCooldownMs
                << "ErrorCount:    " << result.errorCount
                << "Elapsed:       " << result.elapsedMs << "ms";
        }
    }

    QList<QUrl> testUrls()
    {
        static constexpr char const* baseUrl = "https://www.linkedin.com/jobs-guest/jobs/api/seeMoreJobPostings/search?keywords=C%2B%2B&location=Serbia&start=";

        QList<QUrl> urls;

        for (unsigned n = 0; n < linksAmount; n++)
        {
            urls.append(baseUrl + QString::number(n));
        }

        return urls;
    }

private:
    QList<BenchmarkConfig> configurations;
    QList<BenchmarkResult> results;

    RequestManager* requestManager = nullptr;
    QElapsedTimer timer;
    qsizetype configurationIndex = 0;
};

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    BenchmarkRunner runner;
    runner.start();

    return app.exec();
}