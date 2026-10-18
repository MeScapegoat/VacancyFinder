#include <gtest/gtest.h>

#include <QueryParametersPresenter.h>
#include <test_helpers.hpp>

#include <QPushButton>
#include <QSignalSpy>
#include <QTest>


TEST(QueryParameters, MaxResultsSync)
{
    QueryParametersPresenter presenter;
    auto& model = presenter.model().maxResults;
    auto& view = presenter.view().maxResultsInput;

    testModelAndViewSync(model, view);
}

TEST(QueryParameters, KeywordsSync)
{
    QueryParametersPresenter presenter;
    auto& model = presenter.model().keywords;
    auto& view = presenter.view().keywordsInput;

    testModelAndViewSync(model, view);
}

TEST(QueryParameters, LocationSync)
{
    QueryParametersPresenter presenter;
    auto& model = presenter.model().location;
    auto& view = presenter.view().locationInput;

    testModelAndViewSync(model, view);
}

TEST(QueryParameters, SearchRequest)
{
    QueryParametersPresenter presenter;
    auto& view = presenter.view().searchButton;

    testButtonSignalEmitted(presenter, &QueryParametersPresenter::searchRequested, view);
}

TEST(QueryParameters, LoadInterestedRequested)
{
    QueryParametersPresenter presenter;
    auto& view = presenter.view().searchInterestedButton;

    testButtonSignalEmitted(presenter, &QueryParametersPresenter::loadInterestedRequested, view);
}

TEST(QueryParameters, ResetRequest)
{
    QueryParametersPresenter presenter;
    auto& view = presenter.view().resetButton;

    testButtonSignalEmitted(presenter, &QueryParametersPresenter::resetRequested, view);
}