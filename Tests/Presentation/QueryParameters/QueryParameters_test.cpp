#include <gtest/gtest.h>

#include <QueryParametersPresenter.h>
#include <test_helpers.hpp>

#include <QPushButton>
#include <QSignalSpy>
#include <QTest>


TEST(QueryParameters, MaxResultsSync)
{
    QueryParametersPresenter presenter;
    auto& model = presenter.getModel().maxResults;
    auto& view = presenter.getView().maxResultsInput;

    testModelAndViewSync(model, view);
}

TEST(QueryParameters, KeywordsSync)
{
    QueryParametersPresenter presenter;
    auto& model = presenter.getModel().keywords;
    auto& view = presenter.getView().keywordsInput;

    testModelAndViewSync(model, view);
}

TEST(QueryParameters, LocationSync)
{
    QueryParametersPresenter presenter;
    auto& model = presenter.getModel().location;
    auto& view = presenter.getView().locationInput;

    testModelAndViewSync(model, view);
}

TEST(QueryParameters, SearchRequest)
{
    QueryParametersPresenter presenter;
    auto& view = presenter.getView().searchButton;

    testButtonSignalEmitted(presenter, &QueryParametersPresenter::searchRequested, view);
}

TEST(QueryParameters, LoadInterestedRequested)
{
    QueryParametersPresenter presenter;
    auto& view = presenter.getView().searchInterestedButton;

    testButtonSignalEmitted(presenter, &QueryParametersPresenter::loadInterestedRequested, view);
}

TEST(QueryParameters, ResetRequest)
{
    QueryParametersPresenter presenter;
    auto& view = presenter.getView().resetButton;

    testButtonSignalEmitted(presenter, &QueryParametersPresenter::resetRequested, view);
}