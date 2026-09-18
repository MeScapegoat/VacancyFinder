#pragma once
// std

// Qt
#include <QFileSystemWatcher>
#include <QThread>
#include <QLoggingCategory>

// custom


Q_DECLARE_LOGGING_CATEGORY(LoggerLog);

class Logger : public QObject
{
    Q_OBJECT

public:
    Logger();
    ~Logger();

public:
    void setLogRulesPath(const QString&);

public slots:
    void applyLogRules(const QString&);

private:
    QString logRulesPath;
    QFileSystemWatcher logRulesWatcher;
    QThread logThread;
};

void myMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString& msg);