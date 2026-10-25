#include "Logger.h"

#include <QByteArray>
#include <QFile>
#include <QFileInfo>
#include <QLoggingCategory>
#include <QString>
#include <QTextStream>
#include <QThread>

#include <cstdio>

Q_LOGGING_CATEGORY(LoggerLog, "logger");

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

void Logger::setLogRulesPath(const QString& path)
{
    if (!logRulesWatcher.files().empty())
        logRulesWatcher.removePaths(logRulesWatcher.files());

    if (!QFile(path).exists())
    {
        qCCritical(LoggerLog) << "Logging rules file does not exist:" << QFileInfo(path).absoluteFilePath();
        return;
    }
    logRulesPath = path;
    if (!logRulesWatcher.addPath(logRulesPath))
        qCWarning(LoggerLog) << "Could not watch logging rules file:" << QFileInfo(logRulesPath).absoluteFilePath();

    emit applyLogRules(logRulesPath);
}

void Logger::applyLogRules(const QString& path)
{
    QFile logRules(path);
    if (!logRules.open(QIODeviceBase::ReadOnly))
    {
        qCWarning(LoggerLog) << "Could not open logging rules file:" << logRules.fileName() << logRules.errorString();
        return;
    }
    QString rules = QString::fromUtf8(logRules.readAll());
    QLoggingCategory::setFilterRules(rules);
    logRules.close();

    qCInfo(LoggerLog) << "Applied logging rules from" << QFileInfo(path).absoluteFilePath();
}

void myMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    QString logMessage = qFormatLogMessage(type, context, msg);
    QThread* currentThread = QThread::currentThread();
    logMessage.replace("%ThreadName", currentThread->objectName());

#ifdef NDEBUG
    QFile logFile("app.log");
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        const QByteArray error = logFile.errorString().toLocal8Bit();
        const QByteArray formattedMessage = logMessage.toLocal8Bit();
        std::fprintf(stderr, "Could not open app.log for logging: %s\n%s\n", error.constData(), formattedMessage.constData());
        return;
    }

    QTextStream out(&logFile);
#else
    QTextStream out(stdout);
#endif

    out << logMessage << Qt::endl;
}
