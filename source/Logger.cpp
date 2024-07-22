// std

// Qt
#include <QFile>
#include <QDir>

// Custom
#include "Logger.h"

Q_LOGGING_CATEGORY(LinkedLogger, "Logger");

namespace Linked
{
    Logger::Logger()
    {
        const QString defaulMessagePattern = QString(
            "[%{time dd-MM-yyyy h:mm:ss.zzz}] %{if-debug}D%{endif}%{if-info}I%{endif}%{if-warning}W%{endif}%{if-critical}C%{endif}%{if-fatal}F%{endif}: %{function}:%{line} - %{message}");
        qSetMessagePattern(defaulMessagePattern);
        
        QObject::connect(&logRulesWatcher, &QFileSystemWatcher::fileChanged, this, &Logger::applyLogRules, Qt::DirectConnection);

        logRulesWatcher.moveToThread(&thread);
        thread.start();
    }

    void Logger::setLogRulesPath(const QString &path)
    {
        if (!logRulesWatcher.files().empty())
            logRulesWatcher.removePaths(logRulesWatcher.files());
        if (!QFile::exists(path))
        {
            qCCritical(LinkedLogger) << path << "doesnt exists";
            return;
        }
        logRulesPath = path;
        logRulesWatcher.addPath(logRulesPath);

        emit applyLogRules(logRulesPath);
    }

    void Logger::applyLogRules(const QString &path)
    {
        QFile logRules(path);
        if (!logRules.open(QIODeviceBase::ReadOnly))
        {
            qCWarning(LinkedLogger) << "couldn't open" << logRules.fileName();
            return;
        }
        QString rules = QString::fromUtf8(logRules.readAll());
        QLoggingCategory::setFilterRules(rules);
        logRules.close();

        qCInfo(LinkedLogger) << "Log rules updated!";
    }

} // namespace Linked