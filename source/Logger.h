#pragma once
// std

// Qt
#include <QFileSystemWatcher>
#include <QThread>
#include <QLoggingCategory>

// custom


namespace Linked
{
    Q_DECLARE_LOGGING_CATEGORY(LinkedLogger);
    
    class Logger : public QObject
    {
        Q_OBJECT
    public:
        Logger();
        ~Logger();

    public:
        void setLogRulesPath(const QString &);

    public slots:
        void applyLogRules(const QString &);

    private:
        QString logRulesPath;
        QFileSystemWatcher logRulesWatcher;
        QThread logThread;
    };

} // namespace Linked