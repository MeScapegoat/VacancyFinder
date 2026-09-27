#include <gtest/gtest.h>

#include <ResultViewOptionsPresenter.h>
#include <ResultViewOptionsEnums.h>
#include <test_helpers.hpp>


void testObservableAndComboBox(Observable<int>& model, QComboBox& view, int modesAmount)
{
    EXPECT_EQ(view.count(), modesAmount) << "Not all available modes are reflected in UI";
    EXPECT_EQ(model(), view.currentIndex()) << "Initial value of model and view differs";

    model = (model() + 1) & modesAmount;
    EXPECT_EQ(model(), view.currentIndex());

    view.setCurrentIndex((view.currentIndex() + 1) % modesAmount);
    EXPECT_EQ(model(), view.currentIndex());
}


TEST(ResultViewOptions, SortModeSync)
{
    ResultViewOptionsPresenter presenter;
    auto& model = presenter.getModel().currentSortMode;
    auto& view = presenter.getView().sortMode;

    testModelAndViewSync(model, view, static_cast<int>(SortMode::COUNT));
}

TEST(ResultViewOptions, FilterModeSync)
{
    ResultViewOptionsPresenter presenter;
    auto& model = presenter.getModel().currentFilterMode;
    auto& view = presenter.getView().filterMode;

    testModelAndViewSync(model, view, static_cast<int>(FilterMode::COUNT));
}

TEST(ResultViewOptions, FilterTextSync)
{
    ResultViewOptionsPresenter presenter;
    auto& model = presenter.getModel().filterText;
    auto& view = presenter.getView().filterLineEdit;

    testModelAndViewSync(model, view);
}

TEST(ResultViewOptions, UnreviewedStateSync)
{
    ResultViewOptionsPresenter presenter;
    auto& model = presenter.getModel().isUnreviewedVisible;
    auto& view = presenter.getView().unreviewedCheckBox;

    testModelAndViewSync(model, view);
}

TEST(ResultViewOptions, IgnoredStateSync)
{
    ResultViewOptionsPresenter presenter;
    auto& model = presenter.getModel().isIgnoredVisible;
    auto& view = presenter.getView().ignoredCheckBox;

    testModelAndViewSync(model, view);
}

TEST(ResultViewOptions, InterestedStateSync)
{
    ResultViewOptionsPresenter presenter;
    auto& model = presenter.getModel().isInterestedVisible;
    auto& view = presenter.getView().interestedCheckBox;

    testModelAndViewSync(model, view);
}