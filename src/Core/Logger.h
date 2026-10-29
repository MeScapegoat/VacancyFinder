#pragma once

#include <QFileSystemWatcher>
#include <QLoggingCategory>
#include <QObject>

Q_DECLARE_LOGGING_CATEGORY(LoggerLog);

class Logger : public QObject
{
    Q_OBJECT

public:
    Logger();

public:
    void setLogRulesPath(const QString&);

public slots:
    void applyLogRules(const QString&);

private:
    QString logRulesPath;
    QFileSystemWatcher logRulesWatcher;
};

void myMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);
