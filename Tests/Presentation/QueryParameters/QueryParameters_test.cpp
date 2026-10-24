#include <gtest/gtest.h>

#include <QLineEdit>
#include <QPushButton>
#include <QSignalSpy>
#include <QSpinBox>
#include <QString>
#include <QTest>

#include <Observable.hpp>
#include <QueryParametersPresenter.h>
#include <QtTestPrinters.h>


class QueryParametersPresenterTest : public testing::Test, public QueryParametersPresenter
{
protected:
    QueryParametersPresenterTest() :
        keywordsChangedSpy(this, &QueryParametersPresenter::keywordsChanged),
        searchRequestedSpy(this, &QueryParametersPresenter::searchRequested),
        loadInterestedRequestedSpy(this, &QueryParametersPresenter::loadInterestedRequested),
        resetRequestedSpy(this, &QueryParametersPresenter::resetRequested)
    {}

    QSignalSpy keywordsChangedSpy;
    QSignalSpy searchRequestedSpy;
    QSignalSpy loadInterestedRequestedSpy;
    QSignalSpy resetRequestedSpy;
};


TEST_F(QueryParametersPresenterTest, MaxResultsSync)
{
    Observable<int>& maxResults = model().maxResults;
    QSpinBox& maxResultsInput = view().maxResultsInput;
    const int assignedMaxResults = 42;
    const int enteredMaxResults = 123;

    EXPECT_EQ(maxResults(), maxResultsInput.value());

    maxResults = assignedMaxResults;
    EXPECT_EQ(maxResults(), assignedMaxResults);
    EXPECT_EQ(maxResultsInput.value(), assignedMaxResults);

    maxResultsInput.setValue(enteredMaxResults);
    EXPECT_EQ(maxResults(), enteredMaxResults);
    EXPECT_EQ(maxResultsInput.value(), enteredMaxResults);
}

TEST_F(QueryParametersPresenterTest, MaxResultsInputClampsValuesToSupportedRange)
{
    EXPECT_EQ(view().maxResultsInput.minimum(), 1);
    EXPECT_EQ(view().maxResultsInput.maximum(), 999);

    view().maxResultsInput.setValue(0);
    EXPECT_EQ(view().maxResultsInput.value(), 1);
    EXPECT_EQ(maxResults(), 1);

    view().maxResultsInput.setValue(1000);
    EXPECT_EQ(view().maxResultsInput.value(), 999);
    EXPECT_EQ(maxResults(), 999);
}

TEST_F(QueryParametersPresenterTest, KeywordsSync)
{
    Observable<QString>& keywords = model().keywords;
    QLineEdit& keywordsInput = view().keywordsInput;
    const QString assignedKeywords = QStringLiteral("C++");
    const QString typedKeywordSuffix = QStringLiteral(" Qt");
    const QString expectedKeywords = assignedKeywords + typedKeywordSuffix;

    EXPECT_EQ(keywords(), keywordsInput.text());

    keywords = assignedKeywords;
    EXPECT_EQ(keywords(), assignedKeywords);
    EXPECT_EQ(keywordsInput.text(), assignedKeywords);
    ASSERT_EQ(keywordsChangedSpy.count(), 1);
    EXPECT_EQ(keywordsChangedSpy.first().first().toString(), assignedKeywords);

    QTest::keyClicks(&keywordsInput, typedKeywordSuffix);
    EXPECT_EQ(keywords(), expectedKeywords);
    EXPECT_EQ(keywordsInput.text(), expectedKeywords);
    ASSERT_EQ(keywordsChangedSpy.count(), 4);
    EXPECT_EQ(keywordsChangedSpy.last().first().toString(), expectedKeywords);
}

TEST_F(QueryParametersPresenterTest, LocationSync)
{
    Observable<QString>& location = model().location;
    QLineEdit& locationInput = view().locationInput;
    const QString assignedLocation = QStringLiteral("Budapest");
    const QString typedLocation = QStringLiteral("Remote");

    EXPECT_EQ(location(), locationInput.text());

    location = assignedLocation;
    EXPECT_EQ(location(), assignedLocation);
    EXPECT_EQ(locationInput.text(), assignedLocation);

    locationInput.selectAll();
    QTest::keyClicks(&locationInput, typedLocation);
    EXPECT_EQ(location(), typedLocation);
    EXPECT_EQ(locationInput.text(), typedLocation);
}

TEST_F(QueryParametersPresenterTest, PublicAccessorsSync)
{
    const QString expectedKeywords = QStringLiteral("C++");
    const QString expectedLocation = QStringLiteral("Remote");
    const int expectedMaxResults = 42;

    setKeywords(expectedKeywords);
    setLocation(expectedLocation);
    setMaxResults(expectedMaxResults);

    EXPECT_EQ(keywords(), expectedKeywords);
    EXPECT_EQ(location(), expectedLocation);
    EXPECT_EQ(maxResults(), expectedMaxResults);

    EXPECT_EQ(view().keywordsInput.text(), expectedKeywords);
    EXPECT_EQ(view().locationInput.text(), expectedLocation);
    EXPECT_EQ(view().maxResultsInput.value(), expectedMaxResults);
}

TEST_F(QueryParametersPresenterTest, SearchRequest)
{
    QTest::mouseClick(&view().searchButton, Qt::LeftButton);
    EXPECT_EQ(searchRequestedSpy.count(), 1);
}

TEST_F(QueryParametersPresenterTest, LoadInterestedRequested)
{
    QTest::mouseClick(&view().searchInterestedButton, Qt::LeftButton);
    EXPECT_EQ(loadInterestedRequestedSpy.count(), 1);
}

TEST_F(QueryParametersPresenterTest, ResetRequest)
{
    QTest::mouseClick(&view().resetButton, Qt::LeftButton);
    EXPECT_EQ(resetRequestedSpy.count(), 1);
}
