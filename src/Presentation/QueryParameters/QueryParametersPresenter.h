#pragma once

#include <QObject>
#include <QString>

#include <Presenter.hpp>
#include "QueryParametersModel.h"
#include "QueryParametersView.h"

class QueryParametersPresenter : public Presenter<QueryParametersView, QueryParametersModel>
{
    Q_OBJECT

public:
    QueryParametersPresenter(QObject* parent = nullptr);
    ~QueryParametersPresenter() = default;

    void setKeywords(const QString& keywords);
    QString keywords() const;

    void setLocation(const QString& location);
    QString location() const;

    void setMaxResults(int maxResults);
    int maxResults() const;

signals:
    void searchRequested();
    void loadInterestedRequested();
    void resetRequested();

    void keywordsChanged(const QString& keywords);

private slots:
    void onMaxResultsChanged();
    void onKeywordsChanged();
    void onLocationChanged();
};
