#include "ConnectionGroup.h"

#include <gtest/gtest.h>

#include <QObject>

TEST(ConnectionGroupTest, ConnectionRemainsActiveWhileGroupExists)
{
    QObject sender;
    QObject receiver;
    ConnectionGroup group;

    int count = 0;

    group += QObject::connect(&sender, &QObject::objectNameChanged, &receiver, [&count]()
    {
        ++count;
    });

    sender.setObjectName("one");
    EXPECT_EQ(count, 1);

    sender.setObjectName("two");
    EXPECT_EQ(count, 2);
}

TEST(ConnectionGroupTest, DisconnectDisconnectsAllConnections)
{
    QObject sender;
    QObject receiver;
    ConnectionGroup group;

    int count = 0;

    group += QObject::connect(&sender, &QObject::objectNameChanged, &receiver, [&count]()
    {
        ++count;
    });

    sender.setObjectName("one");
    EXPECT_EQ(count, 1);

    group.disconnect();

    sender.setObjectName("two");
    EXPECT_EQ(count, 1);
}

TEST(ConnectionGroupTest, DisconnectCanBeCalledMultipleTimes)
{
    QObject sender;
    QObject receiver;
    ConnectionGroup group;

    int count = 0;

    group += QObject::connect(&sender, &QObject::objectNameChanged, &receiver, [&count]()
    {
        ++count;
    });

    group.disconnect();
    group.disconnect();

    sender.setObjectName("one");

    EXPECT_EQ(count, 0);
}

TEST(ConnectionGroupTest, DisconnectHandlesMultipleConnections)
{
    QObject sender;
    QObject receiver1;
    QObject receiver2;
    ConnectionGroup group;

    int count1 = 0;
    int count2 = 0;

    group += QObject::connect(&sender, &QObject::objectNameChanged, &receiver1, [&count1]()
    {
        ++count1;
    });
    group += QObject::connect(&sender, &QObject::objectNameChanged, &receiver2, [&count2]()
    {
        ++count2;
    });

    sender.setObjectName("one");

    EXPECT_EQ(count1, 1);
    EXPECT_EQ(count2, 1);

    group.disconnect();

    sender.setObjectName("two");

    EXPECT_EQ(count1, 1);
    EXPECT_EQ(count2, 1);
}

TEST(ConnectionGroupTest, DestructorDisconnectsConnections)
{
    QObject sender;
    QObject receiver;
    int count = 0;

    {
        ConnectionGroup group;
        group += QObject::connect(&sender, &QObject::objectNameChanged, &receiver, [&count]()
        {
            ++count;
        });

        sender.setObjectName("one");
        EXPECT_EQ(count, 1);
    }

    sender.setObjectName("two");

    EXPECT_EQ(count, 1);
}

TEST(ConnectionGroupTest, OperatorPlusEqualsReturnsGroup)
{
    QObject sender;
    QObject receiver;
    ConnectionGroup group;

    int count = 0;

    ConnectionGroup& result = group += QObject::connect(&sender, &QObject::objectNameChanged, &receiver, [&count]()
    {
        ++count;
    });

    EXPECT_EQ(&result, &group);

    sender.setObjectName("one");

    EXPECT_EQ(count, 1);
}

TEST(ConnectionGroupTest, DisconnectingOneCopyDisconnectsAllCopies)
{
    QObject sender;
    QObject receiver;
    ConnectionGroup group1;

    int count = 0;

    group1 += QObject::connect(&sender, &QObject::objectNameChanged, &receiver, [&count]()
    {
        ++count;
    });

    ConnectionGroup group2 = group1;

    sender.setObjectName("one");
    EXPECT_EQ(count, 1);

    group2.disconnect();

    sender.setObjectName("two");
    EXPECT_EQ(count, 1);

    group1.disconnect();

    sender.setObjectName("three");
    EXPECT_EQ(count, 1);
}