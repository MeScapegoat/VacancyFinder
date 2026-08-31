#pragma once

#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonObject>
#include <QList>
#include <QtGlobal>

struct BenchmarkConfig
{
    qint64 maxConcurrent;
    qint64 requestsPerSecond;
    qint64 requestCooldownMs;

    bool operator==(const BenchmarkConfig& other) const
    {
        return maxConcurrent == other.maxConcurrent &&
            requestsPerSecond == other.requestsPerSecond &&
            requestCooldownMs == other.requestCooldownMs;
    }
};

struct BenchmarkRun
{
    qint64 run;
    qint64 elapsedMs;
    qint64 errorCount;
};

struct BenchmarkResult
{
    BenchmarkConfig config;
    QList<BenchmarkRun> runs;
};

enum class SortColumn
{
    MaxConcurrent = 1,
    RequestsPerSecond = 2,
    RequestCooldownMs = 3,
    ErrorCount = 4,
    ElapsedMs = 5
};

static void saveResults(const QList<BenchmarkResult>& results, const QString& fileName)
{
    qInfo() << "Saving results...";
    QJsonObject root;
    QJsonArray resultsArray;

    for (const BenchmarkResult& result : results)
    {
        QJsonObject config;

        config["maxConcurrent"] = result.config.maxConcurrent;
        config["requestsPerSecond"] = result.config.requestsPerSecond;
        config["requestCooldownMs"] = result.config.requestCooldownMs;

        QJsonArray runs;

        for (const BenchmarkRun& run : result.runs)
        {
            QJsonObject runObject;

            runObject["run"] = run.run;
            runObject["elapsedMs"] = run.elapsedMs;
            runObject["errorCount"] = run.errorCount;

            runs.append(runObject);
        }

        config["runs"] = runs;
        resultsArray.append(config);
    }

    root["results"] = resultsArray;

    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qWarning() << "Failed to create" << fileName << ":" << file.errorString();
        return;
    }

    file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
}

static double averageElapsedMs(const BenchmarkResult& result)
{
    if (result.runs.isEmpty())
        return 0.0;

    double total = 0.0;

    for (const BenchmarkRun& run : result.runs)
        total += run.elapsedMs;

    return total / result.runs.size();
}

static double averageErrorCount(const BenchmarkResult& result)
{
    if (result.runs.isEmpty())
        return 0.0;

    double total = 0.0;

    for (const BenchmarkRun& run : result.runs)
        total += run.errorCount;

    return total / result.runs.size();
}


static void printResults(QList<BenchmarkResult> results, const SortColumn& sortColumn)
{
    std::sort(results.begin(), results.end(), [sortColumn](const BenchmarkResult& a, const BenchmarkResult& b)
    {
        switch (sortColumn)
        {
            case SortColumn::MaxConcurrent:
            return a.config.maxConcurrent < b.config.maxConcurrent;

            case SortColumn::RequestsPerSecond:
            return a.config.requestsPerSecond < b.config.requestsPerSecond;

            case SortColumn::RequestCooldownMs:
            return a.config.requestCooldownMs < b.config.requestCooldownMs;

            case SortColumn::ErrorCount:
            return averageErrorCount(a) < averageErrorCount(b);

            case SortColumn::ElapsedMs:
            return averageElapsedMs(a) < averageElapsedMs(b);
        }

        return false;
    });

    qInfo().noquote() << "";
    qInfo().noquote() << "===================================================================================";
    qInfo().noquote() << "                         BENCHMARK RESULTS";
    qInfo().noquote() << "===================================================================================";
    qInfo().noquote() << "   # | MaxConcurrent | RPS | Cooldown | Runs | AvgErrors | AvgElapsedMs | AvgSecs |";

    for (qsizetype i = 0; i < results.size(); ++i)
    {
        const BenchmarkResult& result = results[i];

        const double avgErrors = averageErrorCount(result);
        const double avgElapsed = averageElapsedMs(result);

        qInfo().noquote() << "-----------------------------------------------------------------------------------";
        qInfo().noquote() << QString("%1 | %2 | %3 | %4 | %5 | %6 | %7 | %8 |")
            .arg(i + 1, 4)
            .arg(result.config.maxConcurrent, 13)
            .arg(result.config.requestsPerSecond, 3)
            .arg(result.config.requestCooldownMs, 8)
            .arg(result.runs.size(), 4)
            .arg(avgErrors, 9, 'f', 2)
            .arg(avgElapsed, 12, 'f', 2)
            .arg(avgElapsed / 1000.0, 7, 'f', 2);
    }

    qInfo().noquote() << "===================================================================================";
}