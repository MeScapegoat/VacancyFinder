#include <gtest/gtest.h>

#include <QList>
#include <QSignalSpy>
#include <QtTest/QTest>

#include <LinkedInParserStrategy/LinkedInParserStrategy.h>
#include <LinkedInRequestStrategy.h>
#include <LinkedInSearchUrlProvider.h>
#include <NetworkManager.h>
#include <Parser.h>
#include <RequestManager.h>
#include <VacancySearchWorkflow.h>

class VacancySearchWorkflowIntegrationTest : public QObject, public ::testing::Test
{
protected:
    VacancySearchWorkflowIntegrationTest() :
        searchUrlProvider(*new LinkedInSearchUrlProvider),
        requestManager(*new RequestManager(*new LinkedInRequestStrategy(new NetworkManager))),
        parser(*new Parser(*new LinkedInParserStrategy)),
        workflow(*new VacancySearchWorkflow(searchUrlProvider, requestManager, parser, this)),
        allActiveWorkFinishedSpy(&workflow, &VacancySearchWorkflow::allActiveWorkFinished),
        briefReadySpy(&workflow, &VacancySearchWorkflow::briefReady),
        detailsReadySpy(&workflow, &VacancySearchWorkflow::detailsReady),
        vacanciesReadySpy(&workflow, &VacancySearchWorkflow::vacanciesReady)
    {}

    void interrupt()
    {
        emit workflow.interrupt();
    }

    void startSearch(qsizetype maxVacancies)
    {
        workflow.startSearch(QStringLiteral("C++"), QStringLiteral("Belgrade"), maxVacancies);
    }

    bool waitForActiveSearch()
    {
        if (briefReadySpy.count() == 0 && !briefReadySpy.wait(120'000))
        {
            ADD_FAILURE() << "Timed out waiting for a parsed vacancy before interrupt";
            return false;
        }

        if (allActiveWorkFinishedSpy.count() != 0)
        {
            ADD_FAILURE() << "Search completed before the interrupt was sent";
            return false;
        }

        return true;
    }

    void expectSuccessfulSearch(qsizetype minVacancies, int timeoutMilliseconds)
    {
        ASSERT_TRUE(allActiveWorkFinishedSpy.wait(timeoutMilliseconds)) << "Timed out waiting for all active work to finish";

        EXPECT_EQ(allActiveWorkFinishedSpy.count(), 1);
        EXPECT_GE(briefReadySpy.count(), minVacancies);
        EXPECT_EQ(briefReadySpy.count(), detailsReadySpy.count());

        ASSERT_EQ(vacanciesReadySpy.count(), 1);
        const QList<Vacancy> vacancies = vacanciesReadySpy.at(0).at(0).value<QList<Vacancy>>();
        EXPECT_FALSE(vacancies.isEmpty());
        EXPECT_EQ(vacancies.size(), briefReadySpy.count());
    }

    LinkedInSearchUrlProvider& searchUrlProvider;
    RequestManager& requestManager;
    Parser& parser;
    VacancySearchWorkflow& workflow;

    QSignalSpy allActiveWorkFinishedSpy;
    QSignalSpy briefReadySpy;
    QSignalSpy detailsReadySpy;
    QSignalSpy vacanciesReadySpy;
};

TEST_F(VacancySearchWorkflowIntegrationTest, SmallSearch)
{
    constexpr qsizetype maxVacancies = 10;

    startSearch(maxVacancies);

    expectSuccessfulSearch(maxVacancies, 120'000);
}

TEST_F(VacancySearchWorkflowIntegrationTest, MediumSearch)
{
    constexpr qsizetype maxVacancies = 35;

    startSearch(maxVacancies);

    expectSuccessfulSearch(maxVacancies, 240'000);
}

TEST_F(VacancySearchWorkflowIntegrationTest, BigSearch)
{
    constexpr qsizetype maxVacancies = 100;

    startSearch(maxVacancies);

    expectSuccessfulSearch(maxVacancies, 360'000);
}

TEST_F(VacancySearchWorkflowIntegrationTest, InterruptFinishesAllWorkExactlyOnce)
{
    constexpr qsizetype maxVacancies = 100;

    startSearch(maxVacancies);
    QTest::qWait(5'000);

    interrupt();

    if (allActiveWorkFinishedSpy.count() == 0)
        ASSERT_TRUE(allActiveWorkFinishedSpy.wait(240'000)) << "Timed out waiting for all active work to finish after interrupt";

    EXPECT_EQ(allActiveWorkFinishedSpy.count(), 1);

    EXPECT_EQ(allActiveWorkFinishedSpy.count(), 1);
}

TEST_F(VacancySearchWorkflowIntegrationTest, InterruptStopsFurtherParsing)
{
    constexpr qsizetype maxVacancies = 100;

    startSearch(maxVacancies);
    QTest::qWait(5'000);

    interrupt();

    if (allActiveWorkFinishedSpy.count() == 0)
        ASSERT_TRUE(allActiveWorkFinishedSpy.wait(240'000)) << "Timed out waiting for all active work to finish after interrupt";

    const int parsedBriefsAfterInterrupt = briefReadySpy.count();
    const int parsedDetailsAfterInterrupt = detailsReadySpy.count();

    EXPECT_EQ(allActiveWorkFinishedSpy.count(), 1);
    EXPECT_LT(briefReadySpy.count(), maxVacancies);
    EXPECT_EQ(briefReadySpy.count(), parsedBriefsAfterInterrupt);
    EXPECT_EQ(detailsReadySpy.count(), parsedDetailsAfterInterrupt);
}
