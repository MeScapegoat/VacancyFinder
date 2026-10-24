#include <gtest/gtest.h>

#include <QByteArray>
#include <QList>
#include <QNetworkCookie>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSignalSpy>
#include <QUrl>

#include <AbstractNetworkManager.h>
#include <FakeNetworkManager.h>
#include <FakeNetworkReply.h>
#include <LinkedInRequestStrategy.h>

namespace
{
    class RecordingNetworkManager final : public AbstractNetworkManager
    {
    public:
        explicit RecordingNetworkManager(QObject* parent = nullptr) :
            AbstractNetworkManager(parent),
            m_cookieJar(new QNetworkCookieJar(this))
        {}

        QNetworkCookieJar* cookieJar() override
        {
            return m_cookieJar;
        }

        const QNetworkCookieJar* cookieJar() const override
        {
            return m_cookieJar;
        }

        void setCookieJar(QNetworkCookieJar* cookieJar) override
        {
            if (cookieJar == m_cookieJar)
                return;

            m_cookieJar->deleteLater();
            cookieJar->setParent(this);
            m_cookieJar = cookieJar;
        }

        QNetworkReply* get(const QNetworkRequest& request) override
        {
            QNetworkReply* reply = new FakeNetworkReply(
                request,
                200,
                QByteArrayLiteral("<li>Job</li>"),
                QNetworkReply::NoError,
                this);
            m_pendingReplies.append(reply);

            return reply;
        }

        void finishNextReply()
        {
            if (m_pendingReplies.isEmpty())
                return;

            emit finished(m_pendingReplies.takeFirst());
        }

    private:
        QNetworkCookieJar* m_cookieJar;
        QList<QNetworkReply*> m_pendingReplies;
    };

    QUrl briefUrl(int start = 0)
    {
        static const QString url("https://www.linkedin.com/jobs-guest/jobs/api/seeMoreJobPostings/search?keywords=C%2B%2B&location=Belgrade&start=%1");
        return url.arg(start);
    }

    QUrl detailsUrl(int id = 0)
    {
        static const QList<QUrl> urls
        {
            QUrl("https://rs.linkedin.com/jobs/view/c%2B%2B-software-engineer-at-cisco-4404074794"),
            QUrl("https://www.linkedin.com/jobs/view/senior-firmware-engineer-at-amd-4462213699/"),
            QUrl("https://www.linkedin.com/jobs/view/c++-software-engineer-at-w%C3%A4rtsil%C3%A4-4474323610"),
            QUrl("https://www.linkedin.com/jobs/view/software-engineer-backend-at-databricks-4297741862/"),
            QUrl("https://www.linkedin.com/jobs/view/lead-ui-programmer-at-sportspired-games-4464122635/")
        };

        return urls.at(id);
    }

    class LinkedInRequestStrategyTest : public ::testing::Test
    {
    protected:
        LinkedInRequestStrategyTest() :
            manager(new FakeNetworkManager),
            strategy(new LinkedInRequestStrategy(manager)),
            urlsEnqueuedSpy(strategy, &AbstractRequestStrategy::urlsEnqueued),
            urlsDequeuedSpy(strategy, &AbstractRequestStrategy::urlsDequeued),

            startedSpy(strategy, &AbstractRequestStrategy::requestsStarted),
            finishedSpy(strategy, &AbstractRequestStrategy::requestsFinished),

            meaningfulSpy(strategy, &AbstractRequestStrategy::meaningfulRepliesReceived),
            meaninglessSpy(strategy, &AbstractRequestStrategy::meaninglessRepliesReceived),

            vacancyCountSpy(strategy, &AbstractRequestStrategy::vacancyCountDetermined),
            receivedReplySpy(strategy, &AbstractRequestStrategy::receivedReply)
        {
            qRegisterMetaType<QNetworkReply*>();
        }

        ~LinkedInRequestStrategyTest()
        {
            manager->deleteLater();

            strategy = nullptr;
            manager = nullptr;
        }

    protected:
        FakeNetworkManager* manager;
        LinkedInRequestStrategy* strategy;

        QSignalSpy urlsEnqueuedSpy;
        QSignalSpy urlsDequeuedSpy;

        QSignalSpy startedSpy;
        QSignalSpy finishedSpy;

        QSignalSpy meaningfulSpy;
        QSignalSpy meaninglessSpy;

        QSignalSpy vacancyCountSpy;
        QSignalSpy receivedReplySpy;
    };
}

TEST_F(LinkedInRequestStrategyTest, AddUrlEmitsUrlsEnqueued)
{
    strategy->addUrl(briefUrl());

    ASSERT_EQ(urlsEnqueuedSpy.count(), 1);
    EXPECT_EQ(urlsEnqueuedSpy.at(0).at(0).toULongLong(), 1);
}

TEST_F(LinkedInRequestStrategyTest, AddUrlsEmitsNumberOfUrlsEnqueued)
{
    const QList<QUrl> urls
    {
        briefUrl(0),
        briefUrl(10),
        briefUrl(20),
    };

    strategy->addUrls(urls);

    ASSERT_EQ(urlsEnqueuedSpy.count(), 1);
    EXPECT_EQ(urlsEnqueuedSpy.at(0).at(0).toInt(), 3);
}

TEST_F(LinkedInRequestStrategyTest, StartNextSendsRequestStartedAndUrlDequeued)
{
    strategy->addUrl(briefUrl());
    strategy->startNext();

    EXPECT_EQ(startedSpy.count(), 1);
    EXPECT_EQ(urlsDequeuedSpy.count(), 1);
}

TEST_F(LinkedInRequestStrategyTest, BriefReplyWithResultsIsMeaningful)
{
    const QByteArray html = "<html><body><li>Job 1</li></body></html>";

    strategy->addUrl(briefUrl());

    manager->respondWith(200, html);

    strategy->startNext();

    manager->finish();

    EXPECT_EQ(meaningfulSpy.count(), 1);
    EXPECT_EQ(receivedReplySpy.count(), 1);
}

TEST_F(LinkedInRequestStrategyTest, EmptyBriefReplyIsMeaningless)
{
    const QByteArray html = "<html><body>No jobs</body></html>";

    strategy->addUrl(briefUrl());

    manager->respondWith(200, html, QNetworkReply::NoError);

    strategy->startNext();

    manager->finish();

    EXPECT_EQ(meaningfulSpy.count(), 0);
    EXPECT_EQ(meaninglessSpy.count(), 1);
}

TEST_F(LinkedInRequestStrategyTest, SuccessfulDetailsReplyEmitsReceivedReply)
{
    const QByteArray html = "<html><body>Job details</body></html>";

    strategy->addUrl(detailsUrl());

    manager->respondWith(200, html);
    strategy->startNext();

    manager->finish();

    ASSERT_EQ(meaningfulSpy.count(), 1);
    ASSERT_EQ(receivedReplySpy.count(), 1);

    EXPECT_EQ(receivedReplySpy.at(0).at(0).toString(), QString::fromUtf8(html));

    EXPECT_EQ(receivedReplySpy.at(0).at(1).toUrl(), detailsUrl());
}

TEST_F(LinkedInRequestStrategyTest, BriefUrlsAreProcessedBeforeDetailsUrls)
{
    const QList<QUrl> urls
    {
        detailsUrl(),
        briefUrl()
    };

    manager->respondWith(200, "<html><body><li>Job 1</li></body></html>");
    strategy->addUrls(urls);

    strategy->startNext();
    manager->finish();

    ASSERT_EQ(receivedReplySpy.count(), 1);
    EXPECT_EQ(receivedReplySpy.at(0).at(1).toUrl(), briefUrl());

    strategy->startNext();
    manager->finish();

    ASSERT_EQ(receivedReplySpy.count(), 2);
    EXPECT_EQ(receivedReplySpy.at(1).at(1).toUrl(), detailsUrl());
}

TEST(LinkedInRequestStrategyConcurrencyTest, AllowsConcurrentRequestsAfterCookiesAreInitialized)
{
    RecordingNetworkManager* manager = new RecordingNetworkManager;
    LinkedInRequestStrategy strategy(manager, 8, 5000, 3);
    QSignalSpy startedSpy(&strategy, &AbstractRequestStrategy::requestsStarted);

    QNetworkCookie cookie("li_at", "token");
    cookie.setDomain("www.linkedin.com");
    cookie.setPath("/");
    ASSERT_TRUE(manager->cookieJar()->insertCookie(cookie));

    strategy.addUrl(briefUrl(0));
    strategy.startNext();
    ASSERT_EQ(startedSpy.count(), 1);
    manager->finishNextReply();

    strategy.addUrls({briefUrl(10), briefUrl(20)});
    strategy.startNext();
    strategy.startNext();

    EXPECT_EQ(startedSpy.count(), 3);

    manager->finishNextReply();
    manager->finishNextReply();
}

TEST(LinkedInRequestStrategyConcurrencyTest, KeepsRequestsSerializedWithoutCookies)
{
    RecordingNetworkManager* manager = new RecordingNetworkManager;
    LinkedInRequestStrategy strategy(manager, 8, 5000, 3);
    QSignalSpy startedSpy(&strategy, &AbstractRequestStrategy::requestsStarted);

    strategy.addUrls({briefUrl(0), briefUrl(10)});
    strategy.startNext();
    ASSERT_EQ(startedSpy.count(), 1);

    strategy.startNext();
    EXPECT_EQ(startedSpy.count(), 1);

    manager->finishNextReply();
    strategy.startNext();

    EXPECT_EQ(startedSpy.count(), 2);
}

TEST_F(LinkedInRequestStrategyTest, NetworkErrorEmitsMeaninglessReply)
{
    strategy->addUrl(briefUrl());

    manager->respondWith(500, {}, QNetworkReply::InternalServerError);

    strategy->startNext();

    manager->finish();

    EXPECT_EQ(meaninglessSpy.count(), 1);
    EXPECT_EQ(receivedReplySpy.count(), 0);
}

TEST_F(LinkedInRequestStrategyTest, TooManyRequestsStartsCooldown)
{
    const QList<QUrl> urls
    {
        briefUrl(0),
        briefUrl(10),
        briefUrl(20)
    };

    QSignalSpy meaninglessSpy(strategy, &LinkedInRequestStrategy::meaninglessRepliesReceived);

    strategy->addUrls(urls);

    manager->respondWith(429, {}, QNetworkReply::UnknownNetworkError);

    strategy->startNext();

    manager->finish();

    EXPECT_EQ(meaninglessSpy.count(), 1);
    EXPECT_EQ(startedSpy.count(), 1);
    EXPECT_EQ(urlsDequeuedSpy.count(), 1);

    strategy->startNext();

    EXPECT_EQ(startedSpy.count(), 1);
    EXPECT_EQ(urlsDequeuedSpy.count(), 1);
}

TEST_F(LinkedInRequestStrategyTest, ResetReplacesCookieJar)
{
    QNetworkCookieJar* originalJar = manager->cookieJar();

    ASSERT_NE(originalJar, nullptr);

    strategy->reset();

    EXPECT_NE(manager->cookieJar(), originalJar);

    EXPECT_NE(manager->cookieJar(), nullptr);
}

TEST_F(LinkedInRequestStrategyTest, InterruptPreventsNewRequests)
{
    strategy->interrupt();

    strategy->addUrl(briefUrl());

    strategy->startNext();

    EXPECT_EQ(startedSpy.count(), 0);
    EXPECT_EQ(urlsDequeuedSpy.count(), 0);
}

TEST_F(LinkedInRequestStrategyTest, ResetAllowsRequestsAgain)
{
    strategy->interrupt();
    strategy->reset();
    strategy->addUrl(briefUrl());

    strategy->startNext();


    EXPECT_EQ(startedSpy.count(), 1);
    EXPECT_EQ(urlsDequeuedSpy.count(), 1);
}

TEST_F(LinkedInRequestStrategyTest, StartNextDoesNothingWhenIdle)
{
    strategy->startNext();

    EXPECT_EQ(startedSpy.count(), 0);
    EXPECT_EQ(urlsDequeuedSpy.count(), 0);
}

TEST_F(LinkedInRequestStrategyTest, VacancyCountIsDeterminedWhenOnlyDetailsUrlsRemain)
{
    const QList<QUrl> urls
    {
        detailsUrl(0),
        detailsUrl(1),
        detailsUrl(2),
    };

    strategy->addUrls(urls);

    strategy->startNext();

    ASSERT_EQ(vacancyCountSpy.count(), 1);
    EXPECT_EQ(vacancyCountSpy.at(0).at(0).toInt(), 3);
}

TEST_F(LinkedInRequestStrategyTest, VacancyCountIsDeterminedOnlyOnce)
{
    const QList<QUrl> urls
    {
        detailsUrl(0),
        detailsUrl(1),
        detailsUrl(2),
    };

    strategy->addUrls(urls);

    strategy->startNext();
    strategy->startNext();

    EXPECT_EQ(vacancyCountSpy.count(), 1);
}

TEST_F(LinkedInRequestStrategyTest, EmptyBriefReplyRemovesLaterUrlsFromQueue)
{
    const QList<QUrl> urls
    {
        briefUrl(0),
        briefUrl(10),
        briefUrl(20),
        detailsUrl(),
    };

    strategy->addUrls(urls);

    manager->respondWith(200, "<html>No jobs</html>");

    strategy->startNext();
    manager->finish();

    ASSERT_EQ(urlsDequeuedSpy.count(), 2);
    EXPECT_EQ(urlsDequeuedSpy.at(1).at(0).toInt(), 2);
    EXPECT_EQ(meaninglessSpy.count(), 1);
    EXPECT_EQ(receivedReplySpy.count(), 0);

    manager->respondWith(200, "<html><body>Job details</body></html>");
    strategy->startNext();
    manager->finish();

    ASSERT_EQ(receivedReplySpy.count(), 1);
    EXPECT_EQ(receivedReplySpy.at(0).at(1).toUrl(), detailsUrl());
}

TEST_F(LinkedInRequestStrategyTest, ResetClearsTheBriefUrlCutoff)
{
    manager->respondWith(200, "<html>No jobs</html>");
    strategy->addUrl(briefUrl(0));
    strategy->startNext();
    manager->finish();

    ASSERT_EQ(meaninglessSpy.count(), 1);

    strategy->reset();
    strategy->addUrls({briefUrl(10), briefUrl(20)});
    strategy->startNext();
    manager->finish();

    EXPECT_EQ(meaninglessSpy.count(), 2);
    ASSERT_EQ(urlsDequeuedSpy.count(), 4);
    EXPECT_EQ(urlsDequeuedSpy.at(3).at(0).toInt(), 1);
}

TEST_F(LinkedInRequestStrategyTest, InterruptCancelsActiveReplies)
{
    strategy->addUrl(briefUrl());

    strategy->startNext();

    ASSERT_EQ(startedSpy.count(), 1);

    strategy->interrupt();

    ASSERT_EQ(finishedSpy.count(), 1);
    EXPECT_EQ(finishedSpy.at(0).at(0).toInt(), 1);
}

TEST_F(LinkedInRequestStrategyTest, InterruptedReplyIsIgnored)
{
    manager->respondWith(200, "<html><body><li>Job</li></body></html>");
    strategy->addUrl(briefUrl());
    strategy->startNext();
    ASSERT_EQ(startedSpy.count(), 1);

    strategy->interrupt();

    manager->finish();

    EXPECT_EQ(meaningfulSpy.count(), 0);
    EXPECT_EQ(meaninglessSpy.count(), 0);
    EXPECT_EQ(receivedReplySpy.count(), 0);
}

TEST_F(LinkedInRequestStrategyTest, TooManyRequestsRequeuesUrl)
{
    strategy->addUrl(briefUrl());

    manager->respondWith(429, {}, QNetworkReply::UnknownNetworkError);

    strategy->startNext();
    manager->finish();

    EXPECT_EQ(meaninglessSpy.count(), 1);
    EXPECT_EQ(startedSpy.count(), 1);

    EXPECT_EQ(urlsEnqueuedSpy.count(), 2);
}

TEST_F(LinkedInRequestStrategyTest, ResetClearsQueuedUrls)
{
    strategy->addUrl(briefUrl(0));
    strategy->addUrl(detailsUrl(0));

    strategy->reset();
    strategy->startNext();

    EXPECT_EQ(startedSpy.count(), 0);
}

TEST_F(LinkedInRequestStrategyTest, ResetResetsVacancyCountDetermined)
{
    strategy->addUrl(detailsUrl(0));
    strategy->startNext();

    ASSERT_EQ(vacancyCountSpy.count(), 1);

    strategy->reset();

    strategy->addUrl(detailsUrl(0));
    strategy->startNext();

    EXPECT_EQ(vacancyCountSpy.count(), 2);
}

TEST_F(LinkedInRequestStrategyTest, ClassifyUrlWithQueryAsBrief)
{
    EXPECT_EQ(clasifyUrl(briefUrl()), UrlType::Brief);
}

TEST_F(LinkedInRequestStrategyTest, ClassifyUrlWithoutQueryAsDetails)
{
    EXPECT_EQ(clasifyUrl(detailsUrl()), UrlType::Details);
}

TEST_F(LinkedInRequestStrategyTest, GetStartExtractsStartParameter)
{
    EXPECT_EQ(getStart(briefUrl(20)), 20);
}

TEST_F(LinkedInRequestStrategyTest, GetStartReturnsZeroWhenParameterIsMissing)
{
    EXPECT_EQ(getStart(QUrl("https://www.linkedin.com/jobs-guest/jobs/api/seeMoreJobPostings/search?keywords=C%2B%2B")), 0);
}

TEST_F(LinkedInRequestStrategyTest, IsAfterStartReturnsTrueForGreaterStart)
{
    EXPECT_TRUE(isAfterStart(10, briefUrl(20)));
}

TEST_F(LinkedInRequestStrategyTest, IsAfterStartReturnsFalseForEqualStart)
{
    EXPECT_FALSE(isAfterStart(10, briefUrl(10)));
}

TEST_F(LinkedInRequestStrategyTest, IsAfterStartReturnsFalseForSmallerStart)
{
    EXPECT_FALSE(isAfterStart(10, briefUrl(5)));
}
