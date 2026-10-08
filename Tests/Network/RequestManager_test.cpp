#include <gtest/gtest.h>

#include <QSignalSpy>
#include <QUrl>

#include <RequestManager.h>
#include <RequestStrategy/AbstractRequestStrategy.h>
#include <FakeNetworkManager.h>


class MockRequestStrategy final : public AbstractRequestStrategy
{
public:
    explicit MockRequestStrategy(QObject* parent = nullptr) :
        AbstractRequestStrategy(new FakeNetworkManager, parent)
    {}

public:
    void reset() override
    {
        resetCalled = true;
    }

    void addUrls(const QList<QUrl>& urls) override
    {
        addUrlsCalled = true;
        receivedUrls = urls;
    }

    void addUrl(const QUrl& url) override
    {
        addUrlCalled = true;
        receivedUrl = url;
    }

    void interrupt() override
    {
        interruptCalled = true;
    }

    void handleReceivedReply(QNetworkReply* reply) override
    {};

    void startNext() override
    {};

public:
    bool resetCalled = false;
    bool addUrlsCalled = false;
    bool addUrlCalled = false;
    bool interruptCalled = false;

public:
    QList<QUrl> receivedUrls;
    QUrl receivedUrl;
};

class RequestManagerTest : public testing::Test
{
protected:
    RequestManagerTest() :
        m_strategy(),
        m_manager(&m_strategy),
        urlsEnqueuedSpy(&m_manager, &RequestManager::urlsEnqueued),
        urlsDequeuedSpy(&m_manager, &RequestManager::urlsDequeued),
        requestsStartedSpy(&m_manager, &RequestManager::requestsStarted),
        requestsFinishedSpy(&m_manager, &RequestManager::requestsFinished),
        meaningfulRepliesReceivedSpy(&m_manager, &RequestManager::meaningfulRepliesReceived),
        meaninglessRepliesReceivedSpy(&m_manager, &RequestManager::meaninglessRepliesReceived),
        vacancyCountDeterminedSpy(&m_manager, &RequestManager::vacancyCountDetermined),
        receivedReplySpy(&m_manager, &RequestManager::receivedReply)
    {
    }

protected:
    void emitAllStrategySignals(AbstractRequestStrategy& strategy)
    {
        emit strategy.urlsEnqueued(1);
        emit strategy.urlsDequeued(2);
        emit strategy.requestsStarted(3);
        emit strategy.requestsFinished(4);
        emit strategy.meaningfulRepliesReceived(5);
        emit strategy.meaninglessRepliesReceived(6);
        emit strategy.vacancyCountDetermined(7);
        emit strategy.receivedReply(QString("8"), QUrl("9"));
    }

    void verifyNothingReceived()
    {
        EXPECT_EQ(urlsEnqueuedSpy.count(), 0);
        EXPECT_EQ(urlsDequeuedSpy.count(), 0);
        EXPECT_EQ(requestsStartedSpy.count(), 0);
        EXPECT_EQ(requestsFinishedSpy.count(), 0);
        EXPECT_EQ(meaningfulRepliesReceivedSpy.count(), 0);
        EXPECT_EQ(meaninglessRepliesReceivedSpy.count(), 0);
        EXPECT_EQ(vacancyCountDeterminedSpy.count(), 0);
        EXPECT_EQ(receivedReplySpy.count(), 0);
    }

    void verifyAllReceivedManagerSignals()
    {
        ASSERT_EQ(urlsEnqueuedSpy.count(), 1);
        EXPECT_EQ(urlsEnqueuedSpy.at(0).at(0).toInt(), 1);

        ASSERT_EQ(urlsDequeuedSpy.count(), 1);
        EXPECT_EQ(urlsDequeuedSpy.at(0).at(0).toInt(), 2);

        ASSERT_EQ(requestsStartedSpy.count(), 1);
        EXPECT_EQ(requestsStartedSpy.at(0).at(0).toInt(), 3);

        ASSERT_EQ(requestsFinishedSpy.count(), 1);
        EXPECT_EQ(requestsFinishedSpy.at(0).at(0).toInt(), 4);

        ASSERT_EQ(meaningfulRepliesReceivedSpy.count(), 1);
        EXPECT_EQ(meaningfulRepliesReceivedSpy.at(0).at(0).toInt(), 5);

        ASSERT_EQ(meaninglessRepliesReceivedSpy.count(), 1);
        EXPECT_EQ(meaninglessRepliesReceivedSpy.at(0).at(0).toInt(), 6);

        ASSERT_EQ(vacancyCountDeterminedSpy.count(), 1);
        EXPECT_EQ(vacancyCountDeterminedSpy.at(0).at(0).toInt(), 7);

        ASSERT_EQ(receivedReplySpy.count(), 1);
        EXPECT_EQ(receivedReplySpy.at(0).at(0).toString(), QString("8"));
        EXPECT_EQ(receivedReplySpy.at(0).at(1).toUrl(), QUrl("9"));
    }

protected:
    MockRequestStrategy m_strategy;
    RequestManager m_manager;

    QSignalSpy urlsEnqueuedSpy;
    QSignalSpy urlsDequeuedSpy;
    QSignalSpy requestsStartedSpy;
    QSignalSpy requestsFinishedSpy;
    QSignalSpy meaningfulRepliesReceivedSpy;
    QSignalSpy meaninglessRepliesReceivedSpy;
    QSignalSpy vacancyCountDeterminedSpy;
    QSignalSpy receivedReplySpy;
};

// TODO: Not sure about these
//TEST(RequestManagerTestConstructor, AssertOnEmptyStrategy)
//{
//    EXPECT_DEATH(RequestManager(nullptr), "");
//}
//
//TEST(RequestManagerTestConstructor, AssertOnStrategyFromDifferentThread)
//{
//    QThread thread;
//    MockRequestStrategy strategy2;
//
//    ASSERT_TRUE(strategy2.moveToThread(&thread));
//
//    auto create = [](AbstractRequestStrategy& strategy)
//    {
//        return RequestManager(&strategy);
//    };
//
//    EXPECT_DEATH(create(strategy2), "");
//}
//
//TEST_F(RequestManagerTest, AssertOnEmptyStrategy)
//{
//    EXPECT_DEATH(m_manager.setStrategy(nullptr), "");
//}
//
//TEST_F(RequestManagerTest, AssertOnStrategyFromDifferentThread)
//{
//    QThread thread;
//    MockRequestStrategy newStrategy;
//
//    newStrategy.moveToThread(&thread);
//
//    EXPECT_DEATH(m_manager.setStrategy(&newStrategy), "");
//}

TEST_F(RequestManagerTest, ConstructorSetsStrategyAndConnectsSignals)
{
    ASSERT_EQ(m_manager.strategy(), &m_strategy);

    emitAllStrategySignals(m_strategy);
    verifyAllReceivedManagerSignals();
}

TEST_F(RequestManagerTest, SetSameStrategy)
{
    m_manager.setStrategy(&m_strategy);

    ASSERT_EQ(m_manager.strategy(), &m_strategy);

    emitAllStrategySignals(m_strategy);
    verifyAllReceivedManagerSignals();
}

TEST_F(RequestManagerTest, ReplaceStrategy)
{
    MockRequestStrategy newStrategy;

    m_manager.setStrategy(&newStrategy);

    ASSERT_EQ(m_manager.strategy(), &newStrategy);


    emitAllStrategySignals(m_strategy);
    verifyNothingReceived();


    emitAllStrategySignals(newStrategy);
    verifyAllReceivedManagerSignals();
}

TEST_F(RequestManagerTest, Reset)
{
    m_manager.reset();

    EXPECT_TRUE(m_strategy.resetCalled);
}

TEST_F(RequestManagerTest, AddUrls)
{
    const QList<QUrl> urls{
        QUrl("url1"),
        QUrl("url2")
    };

    m_manager.addUrls(urls);

    EXPECT_TRUE(m_strategy.addUrlsCalled);
    EXPECT_EQ(m_strategy.receivedUrls, urls);
}

TEST_F(RequestManagerTest, AddUrl)
{
    const QUrl url("url");

    m_manager.addUrl(url);

    EXPECT_TRUE(m_strategy.addUrlCalled);
    EXPECT_EQ(m_strategy.receivedUrl, url);
}

TEST_F(RequestManagerTest, Interrupt)
{
    m_manager.interrupt();

    EXPECT_TRUE(m_strategy.interruptCalled);
}