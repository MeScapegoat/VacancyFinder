#include "BenchmarkCommonData.h"

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

#include <WorkCoordinator.h>
#include <RequestManager.h>
#include <NetworkManager.h>
#include <RequestStrategy/LinkedInRequestStrategy.h>

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
        networkManager = new NetworkManager(this);
        strategy = new LinkedInRequestStrategy(networkManager, config.requestsPerSecond, config.requestCooldownMs, config.maxConcurrent, this);
        requestManager = new RequestManager(strategy, this);
        workCoordinator = new WorkCoordinator(this);

        QObject::connect(requestManager, &RequestManager::urlsEnqueued, workCoordinator, &WorkCoordinator::workStarted);
        QObject::connect(requestManager, &RequestManager::urlsDequeued, workCoordinator, &WorkCoordinator::workFinished);

        QObject::connect(requestManager, &RequestManager::requestsStarted, workCoordinator, &WorkCoordinator::workStarted);
        QObject::connect(requestManager, &RequestManager::requestsFinished, workCoordinator, &WorkCoordinator::workFinished);

        QObject::connect(requestManager, &RequestManager::meaningfulRepliesReceived, workCoordinator, &WorkCoordinator::workFinished);
        QObject::connect(requestManager, &RequestManager::meaninglessRepliesReceived, workCoordinator, &WorkCoordinator::workFinished);

        connect(workCoordinator, &WorkCoordinator::allActiveWorkFinished, this, [this]()
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
        const BenchmarkConfig config = configurations[configurationIndex];

        ++runIndex;

        qInfo() << "Finished run:";
        qInfo() << "  Run:               " << runIndex << "/" << totalRuns;
        qInfo() << "  MaxConcurrent:     " << config.maxConcurrent;
        qInfo() << "  RequestsPerSecond: " << config.requestsPerSecond;
        qInfo() << "  RequestCooldownMs: " << config.requestCooldownMs;
        qInfo() << "  Elapsed:           " << elapsedMs / 1000 << "secs";

        const BenchmarkRun currentRun{runIndex, elapsedMs};

        results.last().runs.append(currentRun);

        saveResults(results, fileName);


        networkManager->deleteLater();
        networkManager = nullptr;

        strategy->deleteLater();
        strategy = nullptr;

        requestManager->deleteLater();
        requestManager = nullptr;
        
        workCoordinator->deleteLater();
        workCoordinator = nullptr;

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
        static const QString baseUrl = "https://www.linkedin.com/jobs-guest/jobs/api/seeMoreJobPostings/search?keywords=C%2B%2B&location=Serbia&start=%1";

        QList<QUrl> urls;

        for (unsigned n = 0; n < linksAmount; n++)
        {
            urls.append(baseUrl.arg(n));
        }

        return urls;
    }

private:
    WorkCoordinator* workCoordinator = nullptr;


    NetworkManager* networkManager = nullptr;
    LinkedInRequestStrategy* strategy = nullptr;
    RequestManager* requestManager = nullptr;

    QList<BenchmarkConfig> configurations;
    QList<BenchmarkResult> results;

    
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