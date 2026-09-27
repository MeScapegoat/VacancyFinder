#include <gtest/gtest.h>

#include <ResultViewOptionsPresenter.h>
#include <ResultViewOptionsEnums.h>

#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QTest>


void testObservableAndComboBox(Observable<int>& model, QComboBox& view, int modesAmount)
{
    EXPECT_EQ(view.count(), modesAmount) << "Not all available modes are reflected in UI";
    EXPECT_EQ(model(), view.currentIndex()) << "Initial value of model and view differs";

    model = (model() + 1) & modesAmount;
    EXPECT_EQ(model(), view.currentIndex());

    view.setCurrentIndex((view.currentIndex() + 1) % modesAmount);
    EXPECT_EQ(model(), view.currentIndex());
}

void testObservableAndCheckBox(Observable<bool>& model, QCheckBox& view)
{
    EXPECT_EQ(model(), view.isChecked()) << "Initial value of model and view differs";

    model = !model();
    EXPECT_EQ(model(), view.isChecked());

    view.setChecked(!view.isChecked());
    EXPECT_EQ(model(), view.isChecked());
}


TEST(ResultViewOptions_tests, SortModeSync)
{
    ResultViewOptionsPresenter presenter;
    auto& model = presenter.getModel().currentSortMode;
    auto& view = presenter.getView().sortMode;

    testObservableAndComboBox(model, view, static_cast<int>(SortMode::COUNT));
}

TEST(ResultViewOptions_tests, FilterModeSync)
{
    ResultViewOptionsPresenter presenter;
    auto& model = presenter.getModel().currentFilterMode;
    auto& view = presenter.getView().filterMode;

    testObservableAndComboBox(model, view, static_cast<int>(FilterMode::COUNT));
}

TEST(ResultViewOptions_tests, FilterTextSync)
{
    ResultViewOptionsPresenter presenter;
    auto& model = presenter.getModel().filterText;
    auto& view = presenter.getView().filterLineEdit;

    EXPECT_EQ(model(), view.text());

    model = model() + "TEXT";
    EXPECT_EQ(model(), view.text());

    QTest::keyClicks(&view, view.text() + "TEXT");
    EXPECT_EQ(model(), view.text());
}

TEST(ResultViewOptions_tests, UnreviewedStateSync)
{
    ResultViewOptionsPresenter presenter;
    Observable<bool>& model = presenter.getModel().isUnreviewedVisible;
    QCheckBox& view = presenter.getView().unreviewedCheckBox;

    testObservableAndCheckBox(model, view);
}

TEST(ResultViewOptions_tests, IgnoredStateSync)
{
    ResultViewOptionsPresenter presenter;
    Observable<bool>& model = presenter.getModel().isIgnoredVisible;
    QCheckBox& view = presenter.getView().ignoredCheckBox;

    testObservableAndCheckBox(model, view);
}

TEST(ResultViewOptions_tests, InterestedStateSync)
{
    ResultViewOptionsPresenter presenter;
    Observable<bool>& model = presenter.getModel().isInterestedVisible;
    QCheckBox& view = presenter.getView().interestedCheckBox;

    testObservableAndCheckBox(model, view);
}