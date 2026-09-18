#pragma once

#include "../MVP/Presenter.hpp"

#include "QueryParametersModel.h"
#include "QueryParametersView.h"

#include <QObject>
#include <QLoggingCategory>


Q_DECLARE_LOGGING_CATEGORY(QPPLog);


class QueryParametersPresenter : public Presenter<QueryParametersView, QueryParametersModel>
{
    Q_OBJECT

public:
    QueryParametersPresenter(QObject* parent = nullptr);
    ~QueryParametersPresenter() = default;

signals:
    void searchRequested();

private slots:
    void onMaxResultsChanged();
    void onKeywordsChanged();
    void onLocationChanged();
};