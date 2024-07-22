#pragma once
// std

// Qt
#include <QFileSystemWatcher>
#include <QThread>
#include <QLoggingCategory>
// custom

Q_DECLARE_LOGGING_CATEGORY(LinkedLogger);

namespace Linked
{
    class Logger : public QObject
    {
        Q_OBJECT
    public:
        Logger();
        ~Logger() = default;

    public:
        void setLogRulesPath(const QString &);

    public slots:
        void applyLogRules(const QString &);

    private:
        QString logRulesPath;
        QFileSystemWatcher logRulesWatcher;
        QThread thread;
    };

} // namespace Linked