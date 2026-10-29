#include "QueryParametersPresenter.h"

#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>

QueryParametersPresenter::QueryParametersPresenter(QObject* parent) :
    Presenter(parent)
{
    connect(&view().maxResultsInput(), &QSpinBox::valueChanged, &model().maxResults, &Observable<int>::setValue);
    connect(&model().maxResults, &Observable<int>::valueChanged, this, &QueryParametersPresenter::onMaxResultsChanged);

    connect(&view().keywordsInput(), &QLineEdit::textEdited, &model().keywords, &Observable<QString>::setValue);
    connect(&model().keywords, &Observable<QString>::valueChanged, this, &QueryParametersPresenter::onKeywordsChanged);

    connect(&view().locationInput(), &QLineEdit::textEdited, &model().location, &Observable<QString>::setValue);
    connect(&model().location, &Observable<QString>::valueChanged, this, &QueryParametersPresenter::onLocationChanged);

    connect(&view().searchButton(), &QPushButton::released, this, &QueryParametersPresenter::searchRequested);

    connect(&view().searchInterestedButton(), &QPushButton::released, this, &QueryParametersPresenter::loadInterestedRequested);
}

void QueryParametersPresenter::setKeywords(const QString& keywords)
{
    model().keywords = keywords;
}

QString QueryParametersPresenter::keywords() const
{
    return model().keywords();
}

void QueryParametersPresenter::setLocation(const QString& location)
{
    model().location = location;
}

QString QueryParametersPresenter::location() const
{
    return model().location();
}

void QueryParametersPresenter::setMaxResults(int maxResults)
{
    model().maxResults = maxResults;
}

int QueryParametersPresenter::maxResults() const
{
    return model().maxResults();
}

void QueryParametersPresenter::setLoadInterestedButtonVisible(bool isVisible)
{
    view().searchInterestedButton().setVisible(isVisible);
}

void QueryParametersPresenter::onMaxResultsChanged()
{
    QSignalBlocker blocker(view().maxResultsInput());
    view().setMaxResultsInput(model().maxResults());
}

void QueryParametersPresenter::onKeywordsChanged()
{
    view().setKeywordsInput(model().keywords());

    emit keywordsChanged(model().keywords());
}

void QueryParametersPresenter::onLocationChanged()
{
    view().setLocationInput(model().location());
}
