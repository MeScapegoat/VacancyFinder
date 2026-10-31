#pragma once

#include <algorithm>

#include <QByteArray>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>
#include <QSaveFile>
#include <QString>
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
    qint64 requestAttempts;
    qint64 meaningfulReplies;
    qint64 meaninglessReplies;
    qint64 unmatchedAttempts;
    qint64 summariesParsed;
    qint64 detailsParsed;
    qint64 vacanciesCollected;
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
    ElapsedMs = 4
};

static bool saveResults(const QList<BenchmarkResult>& results, const QString& fileName)
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

            runObject["requestAttempts"] = run.requestAttempts;
            runObject["meaningfulReplies"] = run.meaningfulReplies;
            runObject["meaninglessReplies"] = run.meaninglessReplies;
            runObject["unmatchedAttempts"] = run.unmatchedAttempts;
            runObject["summariesParsed"] = run.summariesParsed;
            runObject["detailsParsed"] = run.detailsParsed;
            runObject["vacanciesCollected"] = run.vacanciesCollected;

            runs.append(runObject);
        }

        config["runs"] = runs;
        resultsArray.append(config);
    }

    root["results"] = resultsArray;

    QSaveFile file(fileName);

    if (!file.open(QIODevice::WriteOnly))
    {
        qWarning() << "Failed to create" << fileName << ":" << file.errorString();
        return false;
    }

    const QByteArray json = QJsonDocument(root).toJson(QJsonDocument::Indented);

    if (file.write(json) != json.size())
    {
        qWarning() << "Failed to write" << fileName << ":" << file.errorString();
        return false;
    }

    if (!file.commit())
    {
        qWarning() << "Failed to save" << fileName << ":" << file.errorString();
        return false;
    }

    return true;
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

static QString averageRunMetric(const BenchmarkResult& result, qint64 BenchmarkRun::* metric)
{
    if (result.runs.isEmpty())
        return QStringLiteral("-");

    qint64 total = 0;

    for (const BenchmarkRun& run : result.runs)
        total += run.*metric;

    return QString::number(static_cast<double>(total) / result.runs.size(), 'f', 1);
}

static constexpr int benchmarkTableWidth = 110;


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

            case SortColumn::ElapsedMs:
            return averageElapsedMs(a) < averageElapsedMs(b);
        }

        return false;
    });

    qInfo().noquote() << "";
    qInfo().noquote() << QString(benchmarkTableWidth, QLatin1Char('='));
    const QString title = QStringLiteral("BENCHMARK RESULTS");
    qInfo().noquote() << QString((benchmarkTableWidth - title.size()) / 2, QLatin1Char(' ')) + title;
    qInfo().noquote() << QString(benchmarkTableWidth, QLatin1Char('='));
    qInfo().noquote() << QStringLiteral("%1 | %2 | %3 | %4 | %5 | %6 | %7 | %8 | %9 | %10 |")
        .arg("#", 4)
        .arg("MaxConcurrent", -13)
        .arg("RPS", -3)
        .arg("Cooldown", -8)
        .arg("Runs", -4)
        .arg("AvgAttempts", -11)
        .arg("AvgUseful", -9)
        .arg("AvgNonUseful", -12)
        .arg("AvgElapsedMs", -12)
        .arg("AvgSecs", -7);

    for (qsizetype i = 0; i < results.size(); ++i)
    {
        const BenchmarkResult& result = results[i];

        const double avgElapsed = averageElapsedMs(result);
                              
        qInfo().noquote() << QString(benchmarkTableWidth, QLatin1Char('-'));
        qInfo().noquote() << QString("%1 | %2 | %3 | %4 | %5 | %6 | %7 | %8 | %9 | %10 |")
            .arg(i + 1, 4)
            .arg(result.config.maxConcurrent, 13)
            .arg(result.config.requestsPerSecond, 3)
            .arg(result.config.requestCooldownMs, 8)
            .arg(result.runs.size(), 4)
            .arg(averageRunMetric(result, &BenchmarkRun::requestAttempts), 11)
            .arg(averageRunMetric(result, &BenchmarkRun::meaningfulReplies), 9)
            .arg(averageRunMetric(result, &BenchmarkRun::meaninglessReplies), 12)
            .arg(avgElapsed, 12, 'f', 2)
            .arg(avgElapsed / 1000.0, 7, 'f', 2);
    }

    qInfo().noquote() << QString(benchmarkTableWidth, QLatin1Char('='));
}
