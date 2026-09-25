#pragma once

#include <Presenter.hpp>

#include "ResultViewOptionsModel.h"
#include "ResultViewOptionsView.h"

#include <QLoggingCategory>
#include <QObject>


Q_DECLARE_LOGGING_CATEGORY(RVOPLog);


class ResultViewOptionsPresenter : public Presenter<ResultViewOptionsView, ResultViewOptionsModel>
{
    Q_OBJECT

public:
    ResultViewOptionsPresenter(QObject* parent = nullptr);
    ~ResultViewOptionsPresenter() = default;

private slots:
    void onCurrentSortIndexChanged();

    void onCurrentFilterIndexChanged();
    void onFilterTextChanged();

    void onUnreviewedVisibleChanged();
    void onIgnoredVisibleChanged();
    void onInterestedVisibleChanged();
};