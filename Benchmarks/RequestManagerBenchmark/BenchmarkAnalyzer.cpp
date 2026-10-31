#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfoList>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

#include <algorithm>

#include "BenchmarkCommonData.h"



static BenchmarkConfig readConfig(const QJsonObject& object)
{
    return {
        object["maxConcurrent"].toInt(),
        object["requestsPerSecond"].toInt(),
        object["requestCooldownMs"].toInt()
    };
}

static QList<BenchmarkRun> readRuns(const QJsonArray& array)
{
    QList<BenchmarkRun> runs;

    for (const QJsonValue& value : array)
    {
        const QJsonObject object = value.toObject();

        runs.append({
            object["run"].toInt(),
            object["elapsedMs"].toInteger(),
            object["requestAttempts"].toInteger(),
            object["meaningfulReplies"].toInteger(),
            object["meaninglessReplies"].toInteger(),
            object["unmatchedAttempts"].toInteger(),
            object["summariesParsed"].toInteger(),
            object["detailsParsed"].toInteger(),
            object["vacanciesCollected"].toInteger(),
        });
    }

    return runs;
}

static void readFile(const QString& fileName, QList<BenchmarkResult>& results)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Failed to open" << fileName << ":" << file.errorString();
        return;
    }

    const QJsonDocument document = QJsonDocument::fromJson(file.readAll());

    if (!document.isObject())
    {
        qWarning() << "Invalid JSON:" << fileName;
        return;
    }

    const QJsonArray resultsArray = document.object()["results"].toArray();

    for (const QJsonValue& value : resultsArray)
    {
        const QJsonObject object = value.toObject();
        const BenchmarkConfig config = readConfig(object);
        const QList<BenchmarkRun> runs = readRuns(object["runs"].toArray());

        auto it = std::find_if(results.begin(), results.end(), [&config](const BenchmarkResult& result)
        {
            return result.config == config;
        });

        if (it == results.end())
            results.append({config, runs});
        else
            it->runs.append(runs);
    }
}

int main(int argc, char* argv[])
{
    SortColumn sortColumn = SortColumn::ElapsedMs;

    if (argc > 1)
    {
        bool ok = false;
        const int value = QString::fromLocal8Bit(argv[1]).toInt(&ok);

        if (!ok || value < 1 || value > 4)
        {
            qWarning() << "Invalid sorting column.";
            qWarning() << "Usage: BenchmarkAnalyzer [1-4]";
            qWarning() << "  1 - MaxConcurrent";
            qWarning() << "  2 - RPS";
            qWarning() << "  3 - Cooldown";
            qWarning() << "  4 - ElapsedMs";
            return 1;
        }

        sortColumn = static_cast<SortColumn>(value);
    }

    QDir directory(QDir::currentPath());

    const QFileInfoList files = directory.entryInfoList(
        {"benchmark_*.json"},
        QDir::Files,
        QDir::Name
    );

    if (files.isEmpty())
    {
        qWarning() << "No benchmark_*.json files found.";
        return 1;
    }

    QList<BenchmarkResult> results;

    for (const QFileInfo& file : files)
    {
        qInfo() << "Reading:" << file.fileName();
        readFile(file.filePath(), results);
    }

    if (results.isEmpty())
    {
        qWarning() << "No benchmark results found.";
        return 1;
    }

    if (files.size() > 1)
    {
        const QString outputFileName = QString("benchmark_combined_%1.json").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss-zzz"));

        if (!saveResults(results, outputFileName))
            return 1;

        qInfo() << "Combined results saved to:" << outputFileName;
    }

    printResults(results, sortColumn);

    return 0;
}
