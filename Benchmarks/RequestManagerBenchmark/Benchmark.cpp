#include "BenchmarkCommonData.h"
#include <RequestManager.h>

#include <QCoreApplication>
#include <QDebug>
#include <QElapsedTimer>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QString>
#include <QTimer>

static constexpr qint64 waitBetweenTestsMs = 60 * 1000;
static constexpr qint64 linksAmount = 100;
static constexpr qint64 totalRuns = 5;



class BenchmarkRunner : public QObject
{
public:
    explicit BenchmarkRunner(QObject* parent = nullptr) :
        QObject(parent)
    {
        configurations =
        {
            //{1, 1, 0},

            {2, 2, 2000},
            {2, 2, 3000},

            {3, 3, 2000},

            {4, 4, 2000},
            {4, 4, 3000},

            {4, 6, 2000},

            {5, 5, 1500},
            {5, 5, 2000},
            {5, 5, 2500},
            {5, 5, 3000},
            {5, 5, 5000},

            {5, 6, 2000},

            {6, 5, 2000},

            {6, 6, 1500},
            {6, 6, 2000},
            {6, 6, 2500},
            {6, 6, 3000},
            {6, 6, 5000},

            {6, 7, 2000},

            {7, 6, 2000},

            {7, 7, 1500},
            {7, 7, 2500},
            {7, 7, 3000},
            {7, 7, 5000},

            {8, 6, 2000},

            {8, 8, 3000},
            {8, 8, 5000},
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
        if (configurationIndex >= configurations.size())
        {
            printResults(results, SortColumn::ElapsedMs);
            QCoreApplication::quit();
            return;
        }

        const BenchmarkConfig config = configurations[configurationIndex];

        qInfo() << "";
        qInfo() << "========================================";
        qInfo() << "Starting benchmark";
        qInfo() << "Run:               " << (runIndex + 1) << "/" << totalRuns;
        qInfo() << "MaxConcurrent:     " << config.maxConcurrent;
        qInfo() << "RequestsPerSecond: " << config.requestsPerSecond;
        qInfo() << "RequestCooldownMs: " << config.requestCooldownMs;
        qInfo() << "========================================";

        if (runIndex == 0)
            results.append({config, {}});

        requestManager = new RequestManager(this);

        requestManager->setMaxConcurrent(config.maxConcurrent);
        requestManager->setRequestsPerSecond(config.requestsPerSecond);
        requestManager->setRequestCooldownMs(config.requestCooldownMs);

        connect(requestManager, &RequestManager::requestsFinished, this, [this]()
        {
            static_assert(false); // TODO: Update this to match the new workflow
        });

        timer.start();

        static QList<QUrl> urls = testUrls();

        requestManager->addUrls(urls);
    }

    void finishCurrentBenchmark()
    {
        const qint64 elapsedMs = timer.elapsed();
        const qint64 errorCount = requestManager->getErrorCount();
        const BenchmarkConfig config = configurations[configurationIndex];

        ++runIndex;

        qInfo() << "Finished run:";
        qInfo() << "  Run:               " << runIndex << "/" << totalRuns;
        qInfo() << "  MaxConcurrent:     " << config.maxConcurrent;
        qInfo() << "  RequestsPerSecond: " << config.requestsPerSecond;
        qInfo() << "  RequestCooldownMs: " << config.requestCooldownMs;
        qInfo() << "  errorCount:        " << errorCount;
        qInfo() << "  Elapsed:           " << elapsedMs / 1000 << "secs";

        const BenchmarkRun currentRun{runIndex, elapsedMs, errorCount};

        results.last().runs.append(currentRun);

        saveResults(results, fileName);

        requestManager->deleteLater();
        requestManager = nullptr;

        if (runIndex < totalRuns)
        {
            QTimer::singleShot(waitBetweenTestsMs, this, &BenchmarkRunner::runNext);
            return;
        }

        runIndex = 0;
        ++configurationIndex;

        QTimer::singleShot(waitBetweenTestsMs, this, &BenchmarkRunner::runNext);
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
    qsizetype runIndex = 0;

    QString fileName;
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