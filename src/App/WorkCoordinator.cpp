#include "WorkCoordinator.h"

Q_LOGGING_CATEGORY(WorkCoordinatorLog, "workCoordinator");

WorkCoordinator::WorkCoordinator(QObject* parent) :
    QObject(parent),
    activeWorkAmount(0)
{
    qCDebug(WorkCoordinatorLog) << __FUNCTION__;
}

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
    qCDebug(WorkCoordinatorLog) << "activeWorkAmount:" << activeWorkAmount;

    if (activeWorkAmount == 0)
    {
        qCDebug(WorkCoordinatorLog) << "allActiveWorkFinished";
        emit allActiveWorkFinished();
    }
}
