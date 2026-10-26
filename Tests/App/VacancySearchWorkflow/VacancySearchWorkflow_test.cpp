#include <gtest/gtest.h>

#include <QList>
#include <QObject>
#include <QSignalSpy>
#include <QString>
#include <QtTest/QTest>
#include <QUrl>

#include <AbstractParserStrategy.h>
#include <AbstractRequestStrategy.h>
#include <AbstractSearchUrlProvider.h>
#include <FakeNetworkManager.h>
#include <Parser.h>
#include <RequestManager.h>
#include <VacancySearchWorkflow.h>

namespace
{
    class TestRequestStrategy final : public AbstractRequestStrategy
    {
    public:
        TestRequestStrategy() : AbstractRequestStrategy(new FakeNetworkManager)
        {}

        void startNext() override {}
        void addUrls(const QList<QUrl>&) override {}
        void addUrl(const QUrl&) override {}
        void handleReceivedReply(QNetworkReply*) override {}
    };

    class TestRequestManager final : public RequestManager
    {
        Q_OBJECT

    public:
        TestRequestManager() : RequestManager(*new TestRequestStrategy)
        {}

    public slots:
        void addUrls(const QList<QUrl>& urls) override
        {
            emit urlsAdded(urls);
        }

        void addUrl(const QUrl& url) override
        {
            emit urlAdded(url);
        }

    signals:
        void urlsAdded(const QList<QUrl>& urls);
        void urlAdded(const QUrl& url);
    };

    class TestSearchUrlProvider final : public AbstractSearchUrlProvider
    {
        Q_OBJECT

    public:
        QList<QUrl> createSearchUrls(const QString& keywords,
                                    const QString& location,
                                    qsizetype maxVacancies) const override
        {
            emit searchParametersReceived(keywords, location, maxVacancies);
            return {QUrl(QStringLiteral("https://search.example/generated"))};
        }

    signals:
        void searchParametersReceived(const QString& keywords,
                                      const QString& location,
                                      qsizetype maxVacancies) const;
    };

    class TestParserStrategy final : public AbstractParserStrategy
    {
        Q_OBJECT

    public:
        PageType classifyPage(const QString& html) const override
        {
            emit pageClassified(html);
            return PageType::Unknown;
        }

        QList<Vacancy> parseBriefs(const QString&) const override
        {
            return {};
        }

        Vacancy parseDetails(const QString&, const QUrl&) const override
        {
            return {};
        }

    signals:
        void pageClassified(const QString& html) const;
    };

    class TestParser final : public Parser
    {
        Q_OBJECT

    public:
        explicit TestParser(TestParserStrategy& strategy) : Parser(strategy)
        {}

        void reset() override
        {
            Parser::reset();
            emit resetCalled();
        }

        void interrupt() override
        {
            Parser::interrupt();
            emit interruptCalled();
        }

    signals:
        void resetCalled();
        void interruptCalled();
    };

    Vacancy makeVacancy(const QUrl& url, const QString& title)
    {
        Vacancy vacancy;
        vacancy.url = url;
        vacancy.title = title;
        return vacancy;
    }

    class VacancySearchWorkflowTest : public QObject, public ::testing::Test
    {
    public:
        VacancySearchWorkflowTest() :
            QObject(),
            ::testing::Test(),
            m_searchUrlProvider(*new TestSearchUrlProvider),
            m_requestManager(*new TestRequestManager),
            m_parserStrategy(*new TestParserStrategy),
            m_parser(*new TestParser(m_parserStrategy)),
            m_workflow(*new VacancySearchWorkflow(m_searchUrlProvider, m_requestManager, m_parser, this)),
            m_searchParametersReceivedSpy(&m_searchUrlProvider, &TestSearchUrlProvider::searchParametersReceived),
            m_urlsRequestedSpy(&m_workflow, &VacancySearchWorkflow::urlsRequested),
            m_urlsAddedSpy(&m_requestManager, &TestRequestManager::urlsAdded),
            m_urlAddedSpy(&m_requestManager, &TestRequestManager::urlAdded),
            m_pageClassifiedSpy(&m_parserStrategy, &TestParserStrategy::pageClassified),
            m_vacancyCountDeterminedSpy(&m_workflow, &VacancySearchWorkflow::vacancyCountDetermined),
            m_briefReadySpy(&m_workflow, &VacancySearchWorkflow::briefReady),
            m_detailsReadySpy(&m_workflow, &VacancySearchWorkflow::detailsReady),
            m_requestManagerInterruptSpy(&m_requestManager, &RequestManager::interrupt),
            m_parserInterruptSpy(&m_parser, &TestParser::interruptCalled),
            m_requestManagerResetSpy(&m_requestManager, &RequestManager::reset),
            m_parserResetSpy(&m_parser, &TestParser::resetCalled),
            m_vacanciesReadySpy(&m_workflow, &VacancySearchWorkflow::vacanciesReady),
            m_allActiveWorkFinishedSpy(&m_workflow, &VacancySearchWorkflow::allActiveWorkFinished)
        {}

    protected:
        TestSearchUrlProvider& m_searchUrlProvider;
        TestRequestManager& m_requestManager;
        TestParserStrategy& m_parserStrategy;
        TestParser& m_parser;

        VacancySearchWorkflow& m_workflow;

        QSignalSpy m_searchParametersReceivedSpy;
        QSignalSpy m_urlsRequestedSpy;
        QSignalSpy m_urlsAddedSpy;
        QSignalSpy m_urlAddedSpy;
        QSignalSpy m_pageClassifiedSpy;
        QSignalSpy m_vacancyCountDeterminedSpy;
        QSignalSpy m_briefReadySpy;
        QSignalSpy m_detailsReadySpy;
        QSignalSpy m_requestManagerInterruptSpy;
        QSignalSpy m_parserInterruptSpy;
        QSignalSpy m_requestManagerResetSpy;
        QSignalSpy m_parserResetSpy;
        QSignalSpy m_vacanciesReadySpy;
        QSignalSpy m_allActiveWorkFinishedSpy;
    };
}

TEST_F(VacancySearchWorkflowTest, StartSearchEmitsUrlsRequested)
{
    const QList<QUrl> urls{
        QUrl(QStringLiteral("https://search.example/?page=1")),
        QUrl(QStringLiteral("https://search.example/?page=2"))
    };
    m_workflow.startSearch(urls);

    ASSERT_EQ(m_urlsRequestedSpy.count(), 1);
    EXPECT_EQ(m_urlsRequestedSpy.at(0).at(0).value<QList<QUrl>>(), urls);
}

TEST_F(VacancySearchWorkflowTest, StartSearchWithParametersUsesProviderUrls)
{
    const QString keywords = QStringLiteral("software engineer");
    const QString location = QStringLiteral("Budapest");
    constexpr qsizetype maxVacancies = 25;

    m_workflow.startSearch(keywords, location, maxVacancies);

    ASSERT_EQ(m_searchParametersReceivedSpy.count(), 1);
    EXPECT_EQ(m_searchParametersReceivedSpy.at(0).at(0).toString(), keywords);
    EXPECT_EQ(m_searchParametersReceivedSpy.at(0).at(1).toString(), location);
    EXPECT_EQ(m_searchParametersReceivedSpy.at(0).at(2).toLongLong(), maxVacancies);

    ASSERT_EQ(m_urlsRequestedSpy.count(), 1);
    QTRY_COMPARE(m_urlsAddedSpy.count(), 1);
    EXPECT_EQ(m_urlsAddedSpy.at(0).at(0).value<QList<QUrl>>(), m_urlsRequestedSpy.at(0).at(0).value<QList<QUrl>>());
}

TEST_F(VacancySearchWorkflowTest, UrlsRequestedCallsRequestManagerAddUrls)
{
    const QList<QUrl> urls{
        QUrl(QStringLiteral("https://search.example/?page=1")),
        QUrl(QStringLiteral("https://search.example/?page=2"))
    };
    m_workflow.startSearch(urls);

    QTRY_COMPARE(m_urlsAddedSpy.count(), 1);
    EXPECT_EQ(m_urlsAddedSpy.at(0).at(0).value<QList<QUrl>>(), urls);
}

TEST_F(VacancySearchWorkflowTest, RequestManagerReceivedReplyCallsParserClassifyPage)
{
    const QString html = QStringLiteral("unclassified page");
    emit m_requestManager.receivedReply(html, QUrl(QStringLiteral("https://jobs.example/page")));

    QTRY_COMPARE(m_pageClassifiedSpy.count(), 1);
    EXPECT_EQ(m_pageClassifiedSpy.at(0).at(0).toString(), html);
}

TEST_F(VacancySearchWorkflowTest, RequestManagerVacancyCountDeterminedReachesWorkflow)
{
    constexpr int vacancyCount = 23;
    emit m_requestManager.vacancyCountDetermined(vacancyCount);

    ASSERT_EQ(m_vacancyCountDeterminedSpy.count(), 1);
    EXPECT_EQ(m_vacancyCountDeterminedSpy.at(0).at(0).toInt(), vacancyCount);
}

TEST_F(VacancySearchWorkflowTest, ParserRequestDetailsCallsRequestManagerAddUrl)
{
    const QUrl vacancyUrl(QStringLiteral("https://jobs.example/vacancy"));
    emit m_parser.requestDetails(vacancyUrl);

    QTRY_COMPARE(m_urlAddedSpy.count(), 1);
    EXPECT_EQ(m_urlAddedSpy.at(0).at(0).toUrl(), vacancyUrl);
}

TEST_F(VacancySearchWorkflowTest, ParserBriefParsedEmitsBriefReady)
{
    const Vacancy brief = makeVacancy(QUrl(QStringLiteral("https://jobs.example/brief")), QStringLiteral("Brief"));
    emit m_parser.briefParsed(brief);

    ASSERT_EQ(m_briefReadySpy.count(), 1);
    const Vacancy readyBrief = qvariant_cast<Vacancy>(m_briefReadySpy.at(0).at(0));
    EXPECT_EQ(readyBrief.url, brief.url);
    EXPECT_EQ(readyBrief.title, brief.title);
}

TEST_F(VacancySearchWorkflowTest, ParserDetailsParsedEmitsDetailsReady)
{
    Vacancy details;
    details.url = QUrl(QStringLiteral("https://jobs.example/details"));
    details.description = QStringLiteral("Full description");
    emit m_parser.detailsParsed(details);

    ASSERT_EQ(m_detailsReadySpy.count(), 1);
    const Vacancy readyDetails = qvariant_cast<Vacancy>(m_detailsReadySpy.at(0).at(0));
    EXPECT_EQ(readyDetails.url, details.url);
    EXPECT_EQ(readyDetails.description, details.description);
}

TEST_F(VacancySearchWorkflowTest, InterruptPropagatesToRequestManagerAndParser)
{
    emit m_workflow.interrupt();

    QTRY_COMPARE(m_requestManagerInterruptSpy.count(), 1);
    QTRY_COMPARE(m_parserInterruptSpy.count(), 1);
}

TEST_F(VacancySearchWorkflowTest, ResetPropagatesToRequestManagerAndParser)
{
    emit m_workflow.reset();

    QTRY_COMPARE(m_requestManagerResetSpy.count(), 1);
    QTRY_COMPARE(m_parserResetSpy.count(), 1);
}

TEST_F(VacancySearchWorkflowTest, ResetStartsFreshWorkAndEmitsOnlyItsCompletedVacancies)
{
    const QUrl oldSearchUrl(QStringLiteral("https://search.example/old"));
    const QUrl newSearchUrl(QStringLiteral("https://search.example/new"));
    const QUrl oldVacancyUrl(QStringLiteral("https://jobs.example/old"));
    const QUrl newVacancyUrl(QStringLiteral("https://jobs.example/new"));
    m_workflow.startSearch({oldSearchUrl});
    QTRY_COMPARE(m_urlsAddedSpy.count(), 1);
    emit m_requestManager.urlsEnqueued();
    emit m_parser.briefParsed(makeVacancy(oldVacancyUrl, QStringLiteral("Old vacancy")));
    EXPECT_EQ(m_vacanciesReadySpy.count(), 0);

    emit m_workflow.reset();
    QTRY_COMPARE(m_requestManagerResetSpy.count(), 1);
    QTRY_COMPARE(m_parserResetSpy.count(), 1);

    m_workflow.startSearch({newSearchUrl});
    QTRY_COMPARE(m_urlsAddedSpy.count(), 2);
    emit m_requestManager.urlsEnqueued();

    const Vacancy newBrief = makeVacancy(newVacancyUrl, QStringLiteral("New vacancy"));
    emit m_parser.briefParsed(newBrief);
    emit m_parser.requestDetails(newVacancyUrl);
    QTRY_COMPARE(m_urlAddedSpy.count(), 1);
    emit m_requestManager.urlsEnqueued();

    Vacancy newDetails;
    newDetails.url = newVacancyUrl;
    newDetails.description = QStringLiteral("New vacancy description");
    emit m_parser.detailsParsed(newDetails);

    emit m_requestManager.requestsStarted(2);
    emit m_requestManager.urlsDequeued(2);
    emit m_parser.pageParsingFinished(2);

    ASSERT_EQ(m_vacanciesReadySpy.count(), 1);
    ASSERT_EQ(m_allActiveWorkFinishedSpy.count(), 1);
    const QList<Vacancy> vacancies = m_vacanciesReadySpy.at(0).at(0).value<QList<Vacancy>>();
    ASSERT_EQ(vacancies.size(), 1);
    EXPECT_EQ(vacancies.first().url, newVacancyUrl);
    EXPECT_EQ(vacancies.first().title, newBrief.title);
    EXPECT_EQ(vacancies.first().description, newDetails.description);
}

#include "VacancySearchWorkflow_test.moc"
