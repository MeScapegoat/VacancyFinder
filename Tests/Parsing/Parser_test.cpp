#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <QSignalSpy>
#include <QUrl>

#include <Parser.h>
#include <ParserStrategy/AbstractParserStrategy.h>

using ::testing::_;
using ::testing::Return;
using ::testing::StrictMock;

class MockParserStrategy : public AbstractParserStrategy
{
public:
    MOCK_METHOD(PageType, classifyPage, (const QString& html), (const, override));
    MOCK_METHOD(QList<Vacancy>, parseBriefs, (const QString& html), (const, override));
    MOCK_METHOD(Vacancy, parseDetails, (const QString& html, const QUrl& url), (const, override));
};

class ParserTest : public ::testing::Test
{
protected:
    ParserTest() :
        strategy(*new StrictMock<MockParserStrategy>),
        parser(&strategy),
        startedSpy(&parser, &Parser::pageParsingStarted),
        finishedSpy(&parser, &Parser::pageParsingFinished),
        requestDetailsSpy(&parser, &Parser::requestDetails),
        briefParsedSpy(&parser, &Parser::briefParsed),
        detailsParsedSpy(&parser, &Parser::detailsParsed)
    {

    }

protected:
    StrictMock<MockParserStrategy>& strategy;
    Parser parser;

    QSignalSpy startedSpy;
    QSignalSpy finishedSpy;

    QSignalSpy requestDetailsSpy;

    QSignalSpy briefParsedSpy;
    QSignalSpy detailsParsedSpy;
};

TEST_F(ParserTest, ConstructorSetsStrategy)
{
    EXPECT_EQ(parser.strategy(), &strategy);
}

TEST_F(ParserTest, ProcessesBriefPage)
{
    const QString html = "brief html";
    const QUrl url("url");
    Vacancy vacancy;
    vacancy.url = QUrl("Vacancy url");

    EXPECT_CALL(strategy, classifyPage(html))
        .WillOnce(Return(PageType::Brief));

    EXPECT_CALL(strategy, parseBriefs(html))
        .WillOnce(Return(QList<Vacancy>{vacancy}));

    parser.processPage(html, url);

    EXPECT_EQ(startedSpy.count(), 1);
    EXPECT_EQ(finishedSpy.count(), 1);
    EXPECT_EQ(requestDetailsSpy.count(), 1);
    ASSERT_EQ(briefParsedSpy.count(), 1);

    const Vacancy parsedVacancy = qvariant_cast<Vacancy>(briefParsedSpy.at(0).at(0));

    EXPECT_EQ(parsedVacancy.url, vacancy.url);
    EXPECT_EQ(parsedVacancy.id, 1);
}

TEST_F(ParserTest, DoesNotParseSameBriefPageTwice)
{
    const QString html = "brief html";
    const QUrl url("url");
    Vacancy vacancy;
    vacancy.url = QUrl("Vacancy url");

    EXPECT_CALL(strategy, classifyPage(html))
        .Times(2)
        .WillRepeatedly(Return(PageType::Brief));

    EXPECT_CALL(strategy, parseBriefs(html))
        .WillOnce(Return(QList<Vacancy>{vacancy}));

    parser.processPage(html, url);
    parser.processPage(html, url);

    EXPECT_EQ(briefParsedSpy.count(), 1);
}

TEST_F(ParserTest, DoesNotParseDuplicateBriefs)
{
    const QString html = "brief html";
    const QUrl url1("url 1");
    const QUrl url2("url 2");

    Vacancy first;
    first.url = QUrl("Vacancy url");

    Vacancy duplicate;
    duplicate.url = first.url;

    EXPECT_CALL(strategy, classifyPage(html))
        .Times(2)
        .WillRepeatedly(Return(PageType::Brief));

    EXPECT_CALL(strategy, parseBriefs(html))
        .WillOnce(Return(QList<Vacancy>{first}))
        .WillOnce(Return(QList<Vacancy>{duplicate}));

    parser.processPage(html, url1);
    parser.processPage(html, url2);

    ASSERT_EQ(briefParsedSpy.count(), 1);
    EXPECT_EQ(requestDetailsSpy.count(), 1);

    const Vacancy parsedVacancy = qvariant_cast<Vacancy>(briefParsedSpy.at(0).at(0));
    EXPECT_EQ(parsedVacancy.id, 1);
}

TEST_F(ParserTest, AssignsIncrementingIdsToBriefs)
{
    const QString html = "brief html";
    const QUrl url("url");

    Vacancy first;
    first.url = QUrl("Vacancy url 1");

    Vacancy second;
    second.url = QUrl("Vacancy url 2");

    EXPECT_CALL(strategy, classifyPage(html))
        .WillOnce(Return(PageType::Brief));

    EXPECT_CALL(strategy, parseBriefs(html))
        .WillOnce(Return(QList<Vacancy>{first, second}));

    parser.processPage(html, url);

    ASSERT_EQ(briefParsedSpy.count(), 2);

    const Vacancy parsedFirst = qvariant_cast<Vacancy>(briefParsedSpy.at(0).at(0));
    const Vacancy parsedSecond = qvariant_cast<Vacancy>(briefParsedSpy.at(1).at(0));

    EXPECT_EQ(parsedFirst.id, 1);
    EXPECT_EQ(parsedSecond.id, 2);
}

TEST_F(ParserTest, ProcessesDetailsPage)
{
    const QString html = "details html";
    const QUrl url("Vacancy url");

    Vacancy details;
    details.url = url;

    EXPECT_CALL(strategy, classifyPage(html))
        .WillOnce(Return(PageType::Details));

    EXPECT_CALL(strategy, parseDetails(html, url))
        .WillOnce(Return(details));

    parser.processPage(html, url);

    ASSERT_EQ(detailsParsedSpy.count(), 1);

    const Vacancy parsedDetails = qvariant_cast<Vacancy>(detailsParsedSpy.at(0).at(0));

    EXPECT_EQ(parsedDetails.url, url);
    EXPECT_EQ(parsedDetails.id, 1);
}

TEST_F(ParserTest, DoesNotParseSameDetailsPageTwice)
{
    const QString html = "details html";
    const QUrl url("Vacancy url");

    Vacancy details;
    details.url = url;

    EXPECT_CALL(strategy, classifyPage(html))
        .Times(2)
        .WillRepeatedly(Return(PageType::Details));

    EXPECT_CALL(strategy, parseDetails(html, url))
        .WillOnce(Return(details));

    parser.processPage(html, url);
    parser.processPage(html, url);

    EXPECT_EQ(detailsParsedSpy.count(), 1);
}

TEST_F(ParserTest, UnknownPageTypeDoesNotParseAnything)
{
    const QString html = "unknown html";
    const QUrl url("url");

    EXPECT_CALL(strategy, classifyPage(html))
        .WillOnce(Return(PageType::Unknown));

    parser.processPage(html, url);

    EXPECT_EQ(briefParsedSpy.count(), 0);
    EXPECT_EQ(detailsParsedSpy.count(), 0);
}

TEST_F(ParserTest, InterruptPreventsProcessing)
{
    const QString html = "brief html";
    const QUrl url("url");

    parser.interrupt();

    EXPECT_CALL(strategy, classifyPage(_))
        .Times(0);

    parser.processPage(html, url);

    EXPECT_EQ(startedSpy.count(), 0);
    EXPECT_EQ(finishedSpy.count(), 0);
}

TEST_F(ParserTest, ResetAllowsPreviouslyProcessedPagesToBeProcessedAgain)
{
    const QString html = "brief html";
    const QUrl url("url");

    Vacancy vacancy;
    vacancy.url = QUrl("Vacancy url");

    EXPECT_CALL(strategy, classifyPage(html))
        .Times(2)
        .WillRepeatedly(Return(PageType::Brief));

    EXPECT_CALL(strategy, parseBriefs(html))
        .Times(2)
        .WillRepeatedly(Return(QList<Vacancy>{vacancy}));

    parser.processPage(html, url);

    parser.reset();

    parser.processPage(html, url);

    ASSERT_EQ(briefParsedSpy.count(), 2);

    const Vacancy first = qvariant_cast<Vacancy>(briefParsedSpy.at(0).at(0));
    const Vacancy second = qvariant_cast<Vacancy>(briefParsedSpy.at(1).at(0));

    EXPECT_EQ(first.id, 1);
    EXPECT_EQ(second.id, 1);
}

TEST_F(ParserTest, ResetClearsInterrupt)
{
    const QString html = "brief html";
    const QUrl url("url");

    Vacancy vacancy;
    vacancy.url = QUrl("Vacancy url");

    parser.interrupt();
    parser.reset();

    EXPECT_CALL(strategy, classifyPage(html))
        .WillOnce(Return(PageType::Brief));

    EXPECT_CALL(strategy, parseBriefs(html))
        .WillOnce(Return(QList<Vacancy>{vacancy}));

    parser.processPage(html, url);

    EXPECT_EQ(briefParsedSpy.count(), 1);
}
