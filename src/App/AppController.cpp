#include "AppController.h"

Q_LOGGING_CATEGORY(AppControllerLog, "appController");

AppController::AppController(QObject* parent) :
    QObject(parent),
    activeWorkAmount(0)
{
    qCDebug(AppControllerLog) << __FUNCTION__;
}

void AppController::reset()
{
    activeWorkAmount = 0;
}

void AppController::workStarted(int amount)
{
    activeWorkAmount += amount;

    checkAllWork();
}

void AppController::workFinished(int amount)
{
    activeWorkAmount -= amount;

    checkAllWork();
}

void AppController::checkAllWork()
{
    qCDebug(AppControllerLog) << "activeWorkAmount:" << activeWorkAmount;

    if (activeWorkAmount == 0)
    {
        qCDebug(AppControllerLog) << "allActiveWorkFinished";
        emit allActiveWorkFinished();
    }
}
