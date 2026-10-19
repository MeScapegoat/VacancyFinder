#include <gtest/gtest.h>

#include <QRandomGenerator>
#include <QSignalSpy>
#include <QTest>
#include <QUrlQuery>

#include <WorkCoordinator.h>
#include <LinkedInParserStrategy/LinkedInParserStrategy.h>
#include <NetworkManager.h>
#include <Parser.h>
#include <RequestManager.h>
#include <RequestStrategy/LinkedInRequestStrategy.h>

// TODO: Extract this functionality into a class and test that class instead.

class AppWorkFlowTest : public QObject, public ::testing::Test
{
protected:
    AppWorkFlowTest() :
        networkManager(*new NetworkManager(this)),
        strategy(*new LinkedInRequestStrategy(&networkManager)),
        requestManager(*new RequestManager(&strategy)),
        networkThread(*new QThread(this)),
        parserStrategy(new LinkedInParserStrategy),
        parser(*new Parser(parserStrategy)),
        parserThread(*new QThread(this)),
        workCoordinatorSpy(&workCoordinator, &WorkCoordinator::allActiveWorkFinished),
        parserBriefSpy(&parser, &Parser::briefParsed),
        parserDetailsSpy(&parser, &Parser::detailsParsed)
    {
        parserStrategy->setParent(&parser);
        strategy.setParent(&requestManager);

        networkThread.setObjectName("networkThread");
        requestManager.moveToThread(&networkThread);

        parserThread.setObjectName("parserThread");
        parser.moveToThread(&parserThread);

        QObject::connect(&networkThread, &QThread::finished, &requestManager, &RequestManager::deleteLater);
        QObject::connect(&parserThread, &QThread::finished, &parser, &Parser::deleteLater);

        // RequestManager
        QObject::connect(&requestManager, &RequestManager::requestsStarted, &workCoordinator, &WorkCoordinator::workStarted);
        QObject::connect(&requestManager, &RequestManager::requestsFinished, &workCoordinator, &WorkCoordinator::workFinished);

        //QObject::connect(&requestManager, &RequestManager::meaningfulRepliesReceived, this, &App::onMeaningfulRepliesReceived); // does +1 and then -1 active work, so actually does nothing, hence commented
        QObject::connect(&requestManager, &RequestManager::meaninglessRepliesReceived, &workCoordinator, &WorkCoordinator::workFinished);

        QObject::connect(&requestManager, &RequestManager::receivedReply, &parser,     &Parser::processPage);
        QObject::connect(&requestManager, &RequestManager::urlsEnqueued,  &workCoordinator, &WorkCoordinator::workStarted);
        QObject::connect(&requestManager, &RequestManager::urlsDequeued,  &workCoordinator, &WorkCoordinator::workFinished);


        // Parser
        QObject::connect(&parser, &Parser::requestDetails, &requestManager, &RequestManager::addUrl);
        //QObject::connect(&parser, &HTMLparser::parsingStarted, &workCoordinator, &WorkCoordinator::workStarted);
        QObject::connect(&parser, &Parser::pageParsingFinished, &workCoordinator, &WorkCoordinator::workFinished);

        networkThread.start();
        parserThread.start();
    }

    ~AppWorkFlowTest()
    {
        networkThread.quit();
        networkThread.wait();

        parserThread.quit();
        parserThread.wait();
    }

protected:

    void interrupt()
    {
        QMetaObject::invokeMethod(&parser, [this]()
        {
            parser.interrupt();
        });

        QMetaObject::invokeMethod(&requestManager, [this]()
        {
            requestManager.interrupt();
        });
    }

    void initSearch(int maxVacancies)
    {
        static constexpr const char* BaseUrl = "https://www.linkedin.com/jobs-guest/jobs/api/seeMoreJobPostings/search";
        static constexpr const char* keywords = "C++";
        static constexpr const char* location = "Belgrade";

        workCoordinator.reset();

        QVector<QUrl> urls;

        for (auto n = 0; n < maxVacancies; n += 10)
        {
            QUrl url(BaseUrl);
            QUrlQuery query;
            query.addQueryItem("keywords", QUrl::toPercentEncoding(keywords));
            query.addQueryItem("location", QUrl::toPercentEncoding(location));
            query.addQueryItem("start", QString::number(n));
            url.setQuery(query);

            urls.append(url);
        }

        QMetaObject::invokeMethod(&requestManager, [this, urls]()
        {
            requestManager.addUrls(urls);
        });
    }
protected:
    WorkCoordinator workCoordinator;

    NetworkManager& networkManager;
    LinkedInRequestStrategy& strategy;
    RequestManager& requestManager;
    QThread& networkThread;

    AbstractParserStrategy* parserStrategy;
    Parser& parser;
    QThread& parserThread;

    QSignalSpy workCoordinatorSpy;
    QSignalSpy parserBriefSpy;
    QSignalSpy parserDetailsSpy;
};

TEST_F(AppWorkFlowTest, SmallSearch)
{
    const int maxVacancies = 10;

    initSearch(maxVacancies);

    ASSERT_TRUE(workCoordinatorSpy.wait(120'000)) << "Timed out waiting for all active work to finish";

    EXPECT_EQ(workCoordinatorSpy.count(), 1);
    EXPECT_GE(parserBriefSpy.count(), maxVacancies);
    EXPECT_EQ(parserBriefSpy.count(), parserDetailsSpy.count());
}

TEST_F(AppWorkFlowTest, MediumSearch)
{
    const int maxVacancies = 35;

    initSearch(maxVacancies);

    ASSERT_TRUE(workCoordinatorSpy.wait(120'000)) << "Timed out waiting for all active work to finish";

    EXPECT_EQ(workCoordinatorSpy.count(), 1);
    EXPECT_GE(parserBriefSpy.count(), maxVacancies);
    EXPECT_EQ(parserBriefSpy.count(), parserDetailsSpy.count());
}

TEST_F(AppWorkFlowTest, BigSearch)
{
    const int maxVacancies = 100;

    initSearch(maxVacancies);

    ASSERT_TRUE(workCoordinatorSpy.wait(240'000)) << "Timed out waiting for all active work to finish";

    EXPECT_EQ(workCoordinatorSpy.count(), 1);
    EXPECT_GE(parserBriefSpy.count(), maxVacancies);
    EXPECT_EQ(parserBriefSpy.count(), parserDetailsSpy.count());
}

TEST_F(AppWorkFlowTest, InterruptFinishesAllWorkExactlyOnce)
{
    const int maxVacancies = 35;

    initSearch(maxVacancies);

    const int interruptDelay = 5'000;

    QTest::qWait(interruptDelay);

    interrupt();

    if (workCoordinatorSpy.count() == 0)
        ASSERT_TRUE(workCoordinatorSpy.wait(120'000)) << "Timed out waiting for all active work to finish after interrupt";

    EXPECT_EQ(workCoordinatorSpy.count(), 1);

    QTest::qWait(1000);

    EXPECT_EQ(workCoordinatorSpy.count(), 1);
}

TEST_F(AppWorkFlowTest, InterruptStopsFurtherParsing)
{
    const int maxVacancies = 35;

    initSearch(maxVacancies);

    const int interruptDelay = 5'000;

    QTest::qWait(interruptDelay);

    interrupt();

    if (workCoordinatorSpy.count() == 0)
        ASSERT_TRUE(workCoordinatorSpy.wait(120'000)) << "Timed out waiting for all active work to finish after interrupt";

    const int parsedBriefsAfterInterrupt = parserBriefSpy.count();
    const int parsedDetailsAfterInterrupt = parserDetailsSpy.count();

    QTest::qWait(1000);

    EXPECT_EQ(workCoordinatorSpy.count(), 1);
    EXPECT_EQ(parserBriefSpy.count(), parsedBriefsAfterInterrupt);
    EXPECT_EQ(parserDetailsSpy.count(), parsedDetailsAfterInterrupt);
}
