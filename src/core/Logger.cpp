// std

// Qt
#include <QFile>
#include <QDir>

// Custom
#include "Logger.h"

namespace Linked
{
    Q_LOGGING_CATEGORY(LinkedLogger, "logger");

    Logger::Logger()
    {
        qInstallMessageHandler(myMessageHandler);
        qSetMessagePattern("[%{time dd-MM-yyyy HH:mm:ss:zzz}][%{function}][%ThreadName]: %{message}");

        QObject::connect(&logRulesWatcher, &QFileSystemWatcher::fileChanged, this, &Logger::applyLogRules, Qt::DirectConnection);

        logThread.setObjectName("LogThread");
        logRulesWatcher.moveToThread(&logThread);
        logThread.start();
    }

    Logger::~Logger()
    {
        logThread.quit();
        logThread.wait();
    }

    void Logger::setLogRulesPath(const QString &path)
    {
        if (!logRulesWatcher.files().empty())
            logRulesWatcher.removePaths(logRulesWatcher.files());

        if (!QFile(path).exists())
        {
            qCCritical(LinkedLogger) << QFileInfo(path).absoluteFilePath() << "doesnt exists";
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

void myMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QFile logFile("app.log");
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        return;
    }

    QTextStream out(&logFile);

    auto log_msg = qFormatLogMessage(type, context, msg);
    auto currentThread = QThread::currentThread();
    log_msg.replace("%ThreadName", currentThread->objectName());

    out << log_msg << Qt::endl;
}