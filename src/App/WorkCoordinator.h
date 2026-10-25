#pragma once

#include <QObject>

class WorkCoordinator : public QObject
{
    Q_OBJECT

public:
    WorkCoordinator(QObject* parent = nullptr);

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
