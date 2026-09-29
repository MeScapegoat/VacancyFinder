#include <gtest/gtest.h>

#include <QSignalSpy>
#include <QUrlQuery>
#include <QTest>
#include <QRandomGenerator>

#include <AppController.h>
#include <HTMLparser.h>
#include <RequestManager.h>


class AppWorkFlowTest : public QObject, public ::testing::Test
{
protected:
    AppWorkFlowTest() :
        requestManager(*new RequestManager),
        networkThread(*new QThread(this)),
        parser(*new HTMLparser),
        parserThread(*new QThread(this)),
        controllerSpy(&controller, &AppController::allActiveWorkFinished)
    {
        networkThread.setObjectName("networkThread");
        requestManager.moveToThread(&networkThread);

        parserThread.setObjectName("parserThread");
        parser.moveToThread(&parserThread);

        QObject::connect(&networkThread, &QThread::finished, &requestManager, &RequestManager::deleteLater);
        QObject::connect(&parserThread, &QThread::finished, &parser, &HTMLparser::deleteLater);

        // RequestManager
        QObject::connect(&requestManager, &RequestManager::requestsStarted, &controller, &AppController::workStarted);
        QObject::connect(&requestManager, &RequestManager::requestsFinished, &controller, &AppController::workFinished);

        //QObject::connect(&requestManager, &RequestManager::meaningfulRepliesReceived, this, &App::onMeaningfulRepliesReceived); // does +1 and then -1 active work, so actually does nothing, hence commented
        QObject::connect(&requestManager, &RequestManager::meaninglessRepliesReceived, &controller, &AppController::workFinished);

        QObject::connect(&requestManager, &RequestManager::receivedReply, &parser, &HTMLparser::processHTML);
        QObject::connect(&requestManager, &RequestManager::urlsEnqueued, &controller, &AppController::workStarted);
        QObject::connect(&requestManager, &RequestManager::urlsDequeued, &controller, &AppController::workFinished);


        // Parser
        QObject::connect(&parser, &HTMLparser::sendRequest, &requestManager, &RequestManager::addUrl);
        //QObject::connect(&parser, &HTMLparser::parsingStarted, &controller, &AppController::workStarted);
        QObject::connect(&parser, &HTMLparser::parsingFinished, &controller, &AppController::workFinished);

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
            parser.onInterrupt();
        });

        QMetaObject::invokeMethod(&requestManager, [this]()
        {
            requestManager.onInterrupt();
        });
    }

    void initSearch(int maxVacancies)
    {
        static constexpr const char* BaseUrl = "https://www.linkedin.com/jobs-guest/jobs/api/seeMoreJobPostings/search";
        static constexpr const char* keywords = "C++";
        static constexpr const char* location = "Belgrade";

        controller.reset();

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
    AppController controller;

    RequestManager& requestManager;
    QThread& networkThread;

    HTMLparser& parser;
    QThread& parserThread;

    QSignalSpy controllerSpy;
};

TEST_F(AppWorkFlowTest, SmallSearch)
{
    const int maxVacancies = QRandomGenerator::global()->bounded(1, 10);

    initSearch(maxVacancies);

    ASSERT_TRUE(controllerSpy.wait(480'000)) << "Timed out waiting for all active work to finish";

    EXPECT_EQ(controllerSpy.count(), 1);
    EXPECT_GE(parser.processedDetails().size(), maxVacancies);
}

TEST_F(AppWorkFlowTest, MediumSearch)
{
    const int maxVacancies = QRandomGenerator::global()->bounded(10, 50);

    initSearch(maxVacancies);

    ASSERT_TRUE(controllerSpy.wait(480'000)) << "Timed out waiting for all active work to finish";

    EXPECT_EQ(controllerSpy.count(), 1);
    EXPECT_GE(parser.processedDetails().size(), maxVacancies);
}

TEST_F(AppWorkFlowTest, BigSearch)
{
    const int maxVacancies = QRandomGenerator::global()->bounded(50, 100);

    initSearch(maxVacancies);

    ASSERT_TRUE(controllerSpy.wait(480'000)) << "Timed out waiting for all active work to finish";

    EXPECT_EQ(controllerSpy.count(), 1);
    EXPECT_GE(parser.processedDetails().size(), maxVacancies);
}

TEST_F(AppWorkFlowTest, InterruptFinishesAllWorkExactlyOnce)
{
    const int maxVacancies = QRandomGenerator::global()->bounded(10, 100);

    initSearch(maxVacancies);

    const int interruptDelay = QRandomGenerator::global()->bounded(1000, 10'000);

    QTest::qWait(interruptDelay);

    interrupt();

    ASSERT_TRUE(controllerSpy.wait(480'000)) << "Timed out waiting for all active work to finish after interrupt";

    EXPECT_EQ(controllerSpy.count(), 1);

    QTest::qWait(1000);

    EXPECT_EQ(controllerSpy.count(), 1);
}

TEST_F(AppWorkFlowTest, InterruptStopsFurtherParsing)
{
    const int maxVacancies = QRandomGenerator::global()->bounded(10, 100);

    initSearch(maxVacancies);

    const int interruptDelay = QRandomGenerator::global()->bounded(1000, 10'000);

    QTest::qWait(interruptDelay);

    interrupt();

    ASSERT_TRUE(controllerSpy.wait(480'000)) << "Timed out waiting for all active work to finish after interrupt";

    const int parsedAfterInterrupt = parser.processedDetails().count();

    QTest::qWait(1000);

    EXPECT_EQ(controllerSpy.count(), 1);
    EXPECT_EQ(parser.processedDetails().count(), parsedAfterInterrupt);
}