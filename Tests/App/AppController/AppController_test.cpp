#include <gtest/gtest.h>
#include <QSignalSpy>

#include <AppController.h>

TEST(AppControllerTest, EmitsWhenAllWorkFinishes)
{
    AppController controller;
    QSignalSpy spy(&controller, &AppController::allActiveWorkFinished);

    controller.workStarted(2);

    controller.workFinished(1);
    EXPECT_EQ(spy.count(), 0);

    controller.workFinished(1);
    EXPECT_EQ(spy.count(), 1);
}

TEST(AppControllerTest, DoesNotEmitWhileWorkIsActive)
{
    AppController controller;
    QSignalSpy spy(&controller, &AppController::allActiveWorkFinished);

    controller.workStarted(3);
    controller.workFinished(1);

    EXPECT_EQ(spy.count(), 0);
}

TEST(AppControllerTest, MultipleWorkStartsAreAccumulated)
{
    AppController controller;
    QSignalSpy spy(&controller, &AppController::allActiveWorkFinished);

    controller.workStarted(2);
    controller.workStarted(3);

    controller.workFinished(4);
    EXPECT_EQ(spy.count(), 0);

    controller.workFinished(1);
    EXPECT_EQ(spy.count(), 1);
}

TEST(AppControllerTest, ResetClearsActiveWork)
{
    AppController controller;
    QSignalSpy spy(&controller, &AppController::allActiveWorkFinished);

    controller.workStarted(5);
    controller.reset();

    EXPECT_EQ(spy.count(), 0);
}