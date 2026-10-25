#include "WorkCoordinator.h"

WorkCoordinator::WorkCoordinator(QObject* parent) :
    QObject(parent),
    activeWorkAmount(0)
{}

void WorkCoordinator::reset()
{
    activeWorkAmount = 0;
}

void WorkCoordinator::workStarted(int amount)
{
    activeWorkAmount += amount;

    checkAllWork();
}

void WorkCoordinator::workFinished(int amount)
{
    activeWorkAmount -= amount;

    checkAllWork();
}

void WorkCoordinator::checkAllWork()
{
    if (activeWorkAmount == 0)
    {
        emit allActiveWorkFinished();
    }
}
