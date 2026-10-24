
#include <gtest/gtest.h>

#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QList>
#include <QSignalSpy>
#include <QString>
#include <QStringList>

#include <CallableComboBox.hpp>
#include <QtTestPrinters.h>
#include <ResultViewOptionsEnums.h>
#include "ResultViewOptionsPresenter.h"
#include <Vacancy.h>
#include <VacancyStatus.h>

class ResultViewOptionsPresenterTest : public testing::Test, public ResultViewOptionsPresenter
{
protected:
    ResultViewOptionsPresenterTest() :
        resultViewOptionsChangedSpy(this, &ResultViewOptionsPresenter::resultViewOptionsChanged),
        vacancySortComparatorChangedSpy(this, &ResultViewOptionsPresenter::vacancySortComparatorChanged),
        vacancyFilterChangedSpy(this, &ResultViewOptionsPresenter::vacancyFilterChanged),
        visibleStatusesChangedSpy(this, &ResultViewOptionsPresenter::visibleStatusesChanged)
    {}

    QSignalSpy resultViewOptionsChangedSpy;
    QSignalSpy vacancySortComparatorChangedSpy;
    QSignalSpy vacancyFilterChangedSpy;
    QSignalSpy visibleStatusesChangedSpy;
};


TEST_F(ResultViewOptionsPresenterTest, InitialViewStateMatchesModelState)
{
    EXPECT_EQ(view().filterMode().currentIndex(), filterMode());
    EXPECT_EQ(view().sortMode().currentIndex(), sortMode());
    EXPECT_EQ(view().filterLineEdit().text(), filterWords());

    EXPECT_EQ(view().unreviewedCheckBox().isChecked(), isUnreviewedVisible());
    EXPECT_EQ(view().ignoredCheckBox().isChecked(), isIgnoredVisible());
    EXPECT_EQ(view().interestedCheckBox().isChecked(), isInterestedVisible());
}


TEST_F(ResultViewOptionsPresenterTest, FilterComboBoxContainsEveryFilterMode)
{
    EXPECT_EQ(view().filterMode().count(), static_cast<int>(FilterMode::COUNT));
}

TEST_F(ResultViewOptionsPresenterTest, SortComboBoxContainsEverySortMode)
{
    EXPECT_EQ(view().sortMode().count(), static_cast<int>(SortMode::COUNT));
}

TEST_F(ResultViewOptionsPresenterTest, ChangingFilterModeSynchronizesViewAndModel)
{
    const int index1 = (filterMode() + 1) % static_cast<int>(FilterMode::COUNT);

    setFilterMode(index1);
    EXPECT_EQ(filterMode(), index1);
    EXPECT_EQ(model().currentFilterMode(), index1);
    EXPECT_EQ(view().filterMode().currentIndex(), index1);
    EXPECT_EQ(vacancyFilterChangedSpy.count(), 1);
    EXPECT_EQ(resultViewOptionsChangedSpy.count(), 1);

    const int index2 = (index1 + 1) % static_cast<int>(FilterMode::COUNT);

    view().filterMode().setCurrentIndex(index2);
    EXPECT_EQ(filterMode(), index2);
    EXPECT_EQ(model().currentFilterMode(), index2);
    EXPECT_EQ(view().filterMode().currentIndex(), index2);
    EXPECT_EQ(vacancyFilterChangedSpy.count(), 2);
    EXPECT_EQ(resultViewOptionsChangedSpy.count(), 2);
}

TEST_F(ResultViewOptionsPresenterTest, ChangingFilterWordsSynchronizesViewAndModel)
{
    const QString filterWords1 = "C++, Qt, developer";
    const QStringList filterWordList1 = QStringList({"C++", "Qt", "developer"});

    setFilterWords(filterWords1);
    EXPECT_EQ(filterWords(), filterWords1);
    EXPECT_EQ(model().filterWordList(), filterWordList1);
    EXPECT_EQ(view().filterLineEdit().text(), filterWords1);
    EXPECT_EQ(resultViewOptionsChangedSpy.count(), 1);


    const QString filterWords2 = "Python, Rust";
    const QStringList filterWordList2 = QStringList({"Python", "Rust"});

    view().filterLineEdit().setText(filterWords2);
    EXPECT_EQ(filterWords(), filterWords2);
    EXPECT_EQ(model().filterWordList(), filterWordList2);
    EXPECT_EQ(view().filterLineEdit().text(), filterWords2);
    EXPECT_EQ(resultViewOptionsChangedSpy.count(), 2);
}
TEST_F(ResultViewOptionsPresenterTest, ChangingSortModeSynchronizesViewAndModel)
{
    const int index1 = (sortMode() + 1) % static_cast<int>(SortMode::COUNT);

    setSortMode(index1);
    EXPECT_EQ(sortMode(), index1);
    EXPECT_EQ(model().currentSortMode(), index1);
    EXPECT_EQ(view().sortMode().currentIndex(), index1);
    EXPECT_EQ(vacancySortComparatorChangedSpy.count(), 1);
    EXPECT_EQ(resultViewOptionsChangedSpy.count(), 1);


    const int index2 = (index1 + 1) % static_cast<int>(SortMode::COUNT);

    view().sortMode().setCurrentIndex(index2);
    EXPECT_EQ(sortMode(), index2);
    EXPECT_EQ(model().currentSortMode(), index2);
    EXPECT_EQ(view().sortMode().currentIndex(), index2);
    EXPECT_EQ(vacancySortComparatorChangedSpy.count(), 2);
    EXPECT_EQ(resultViewOptionsChangedSpy.count(), 2);
}

TEST_F(ResultViewOptionsPresenterTest, ChangingUnreviewedVisibilitySynchronizesViewAndModel)
{
    const bool value1 = !isUnreviewedVisible();
    setIsUnreviewedVisible(value1);
    EXPECT_EQ(isUnreviewedVisible(), value1);
    EXPECT_EQ(view().unreviewedCheckBox().isChecked(), value1);
    EXPECT_EQ(model().isUnreviewedVisible(), value1);
    EXPECT_EQ(resultViewOptionsChangedSpy.count(), 1);


    const bool value2 = !value1;
    view().unreviewedCheckBox().setChecked(value2);
    EXPECT_EQ(isUnreviewedVisible(), value2);
    EXPECT_EQ(view().unreviewedCheckBox().isChecked(), value2);
    EXPECT_EQ(model().isUnreviewedVisible(), value2);
    EXPECT_EQ(resultViewOptionsChangedSpy.count(), 2);
}

TEST_F(ResultViewOptionsPresenterTest, ChangingIgnoredVisibilitySynchronizesViewAndModel)
{
    const bool value1 = !isIgnoredVisible();
    setIsIgnoredVisible(value1);
    EXPECT_EQ(isIgnoredVisible(), value1);
    EXPECT_EQ(model().isIgnoredVisible(), value1);
    EXPECT_EQ(view().ignoredCheckBox().isChecked(), value1);
    EXPECT_EQ(resultViewOptionsChangedSpy.count(), 1);


    const bool value2 = !value1;
    view().ignoredCheckBox().setChecked(value2);
    EXPECT_EQ(isIgnoredVisible(), value2);
    EXPECT_EQ(model().isIgnoredVisible(), value2);
    EXPECT_EQ(view().ignoredCheckBox().isChecked(), value2);
    EXPECT_EQ(resultViewOptionsChangedSpy.count(), 2);
}

TEST_F(ResultViewOptionsPresenterTest, ChangingInterestedVisibilitySynchronizesViewAndModel)
{
    const bool value1 = !isInterestedVisible();
    setIsInterestedVisible(value1);
    EXPECT_EQ(isInterestedVisible(), value1);
    EXPECT_EQ(model().isInterestedVisible(), value1);
    EXPECT_EQ(view().interestedCheckBox().isChecked(), value1);
    EXPECT_EQ(resultViewOptionsChangedSpy.count(), 1);


    const bool value2 = !value1;
    view().interestedCheckBox().setChecked(!isInterestedVisible());
    EXPECT_EQ(isInterestedVisible(), value2);
    EXPECT_EQ(model().isInterestedVisible(), value2);
    EXPECT_EQ(view().interestedCheckBox().isChecked(), value2);
    EXPECT_EQ(resultViewOptionsChangedSpy.count(), 2);
}

TEST_F(ResultViewOptionsPresenterTest, ChangingFilterModeUpdatesVacancyFilter)
{
    setFilterMode(static_cast<int>(FilterMode::COUNT) - 1);
    vacancyFilterChangedSpy.clear();

    for (int i = 0; i < static_cast<int>(FilterMode::COUNT); ++i)
    {
        setFilterMode(i);

        EXPECT_EQ(&view().filterMode().currentFunction(), &vacancyFilter());
        EXPECT_TRUE(vacancyFilter());
    }

    EXPECT_EQ(vacancyFilterChangedSpy.count(), static_cast<int>(FilterMode::COUNT));
}

TEST_F(ResultViewOptionsPresenterTest, FilterModeCallbacksApplySelectedRules)
{
    Vacancy matchesOneWord{};
    matchesOneWord.title = "C++ Developer";

    Vacancy matchesAllWords{};
    matchesAllWords.title = "C++ Qt Developer";

    Vacancy matchesNoWords{};
    matchesNoWords.title = "Java Developer";

    setFilterWords("C++, Qt");

    setFilterMode(static_cast<int>(FilterMode::ContainsAny));
    EXPECT_TRUE(vacancyFilter()(&matchesOneWord));
    EXPECT_FALSE(vacancyFilter()(&matchesNoWords));

    setFilterMode(static_cast<int>(FilterMode::ContainsAll));
    EXPECT_TRUE(vacancyFilter()(&matchesAllWords));
    EXPECT_FALSE(vacancyFilter()(&matchesOneWord));

    setFilterMode(static_cast<int>(FilterMode::ExcludesAny));
    EXPECT_TRUE(vacancyFilter()(&matchesNoWords));
    EXPECT_FALSE(vacancyFilter()(&matchesOneWord));

    setFilterMode(static_cast<int>(FilterMode::ExcludesAll));
    EXPECT_TRUE(vacancyFilter()(&matchesOneWord));
    EXPECT_FALSE(vacancyFilter()(&matchesAllWords));

    setFilterWords("Java");
    EXPECT_FALSE(vacancyFilter()(&matchesNoWords));
    EXPECT_TRUE(vacancyFilter()(&matchesOneWord));
}

TEST_F(ResultViewOptionsPresenterTest, ChangingSortModeUpdatesVacancySortComparator)
{
    setSortMode(static_cast<int>(SortMode::COUNT) - 1);
    vacancySortComparatorChangedSpy.clear();

    for (int i = 0; i < static_cast<int>(SortMode::COUNT); ++i)
    {
        setSortMode(i);

        EXPECT_EQ(&view().sortMode().currentFunction(), &vacancySortComparator());

        if (i == static_cast<int>(SortMode::Off))
            EXPECT_FALSE(vacancySortComparator());
        else
            EXPECT_TRUE(vacancySortComparator());
    }

    EXPECT_EQ(vacancySortComparatorChangedSpy.count(), static_cast<int>(SortMode::COUNT));
}

TEST_F(ResultViewOptionsPresenterTest, RelevanceSortUsesCurrentKeywords)
{
    Vacancy cppVacancy{};
    cppVacancy.title = "C++ C++ Developer";

    Vacancy qtVacancy{};
    qtVacancy.title = "Qt Developer";

    setSortMode(static_cast<int>(SortMode::Relevance));
    setKeywords("C++");
    EXPECT_TRUE(vacancySortComparator()(&cppVacancy, &qtVacancy));

    setKeywords("Qt");
    EXPECT_FALSE(vacancySortComparator()(&cppVacancy, &qtVacancy));
    EXPECT_TRUE(vacancySortComparator()(&qtVacancy, &cppVacancy));
}

TEST_F(ResultViewOptionsPresenterTest, VisibleStatusesMatchVisibilitySettings)
{
    for (int mask = 0; mask < 8; ++mask)
    {
        const bool unreviewed = mask & 1;
        const bool interested = mask & 2;
        const bool ignored = mask & 4;

        setIsUnreviewedVisible(unreviewed);
        setIsInterestedVisible(interested);
        setIsIgnoredVisible(ignored);

        QList<VacancyStatus> expected;
        if (unreviewed) expected.push_back(VacancyStatus::Unreviewed);
        if (ignored) expected.push_back(VacancyStatus::Ignored);
        if (interested) expected.push_back(VacancyStatus::Interested);

        EXPECT_EQ(visibleStatuses(), expected);
    }
}

TEST_F(ResultViewOptionsPresenterTest, SettingKeywordsUpdatesModel)
{
    const QString keywords = "C++ Qt developer";
    setKeywords(keywords);
    EXPECT_EQ(model().keywords(), keywords);
}

TEST(ResultViewOptionsFreeFunctionsTest, FilterWordsToStringJoinsWordsWithCommaAndSpace)
{
    EXPECT_EQ(filterWordsToString({"C++", "Qt", "developer"}), QString("C++, Qt, developer"));
}

TEST(ResultViewOptionsFreeFunctionsTest, FilterWordsToListSplitsAndTrimsWords)
{
    EXPECT_EQ(filterWordsToList(" C++, Qt , developer "), (QStringList{"C++", "Qt", "developer"}));
}

TEST(ResultViewOptionsFreeFunctionsTest, FilterWordsToStringReturnsEmptyStringForEmptyList)
{
    EXPECT_EQ(filterWordsToString({}), QString(""));
}

TEST(ResultViewOptionsFreeFunctionsTest, FilterWordsToListPreservesEmptyElements)
{
    EXPECT_EQ(filterWordsToList("C++,,Qt"), (QStringList{"C++", "", "Qt"}));
}

TEST(ResultViewOptionsFreeFunctionsTest, FilterWordsToListTrimsWhitespaceAroundEmptyElements)
{
    EXPECT_EQ(filterWordsToList(" C++ , , Qt "), (QStringList{"C++", "", "Qt"}));
}
