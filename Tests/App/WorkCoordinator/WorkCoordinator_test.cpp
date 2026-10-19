#include <gtest/gtest.h>
#include <QSignalSpy>

#include <WorkCoordinator.h>

TEST(WorkCoordinatorTest, EmitsWhenAllWorkFinishes)
{
    WorkCoordinator workCoordinator;
    QSignalSpy spy(&workCoordinator, &WorkCoordinator::allActiveWorkFinished);

    workCoordinator.workStarted(2);

    workCoordinator.workFinished(1);
    EXPECT_EQ(spy.count(), 0);

    workCoordinator.workFinished(1);
    EXPECT_EQ(spy.count(), 1);
}

TEST(WorkCoordinatorTest, DoesNotEmitWhileWorkIsActive)
{
    WorkCoordinator workCoordinator;
    QSignalSpy spy(&workCoordinator, &WorkCoordinator::allActiveWorkFinished);

    workCoordinator.workStarted(3);
    workCoordinator.workFinished(1);

    EXPECT_EQ(spy.count(), 0);
}

TEST(WorkCoordinatorTest, MultipleWorkStartsAreAccumulated)
{
    WorkCoordinator workCoordinator;
    QSignalSpy spy(&workCoordinator, &WorkCoordinator::allActiveWorkFinished);

    workCoordinator.workStarted(2);
    workCoordinator.workStarted(3);

    workCoordinator.workFinished(4);
    EXPECT_EQ(spy.count(), 0);

    workCoordinator.workFinished(1);
    EXPECT_EQ(spy.count(), 1);
}

TEST(WorkCoordinatorTest, ResetClearsActiveWork)
{
    WorkCoordinator workCoordinator;
    QSignalSpy spy(&workCoordinator, &WorkCoordinator::allActiveWorkFinished);

    workCoordinator.workStarted(5);
    workCoordinator.reset();

    EXPECT_EQ(spy.count(), 0);
}