#pragma once

#include <QObject>

#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(AppControllerLog);

class AppController : public QObject
{
    Q_OBJECT

public:
    AppController(QObject* parent = nullptr);

public:
    void reset();

public slots:
    void workStarted(int amount);
    void workFinished(int amount);

signals:
    void allActiveWorkFinished();

private:
    void checkAllWork();

private:
    int activeWorkAmount;
};