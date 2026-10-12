#pragma once

#include <Presenter.hpp>

#include "ResultViewOptionsModel.h"
#include "ResultViewOptionsView.h"

#include <QLoggingCategory>
#include <QObject>

// TODO: TMP CODE, REMOVE
#include "ResultViewOptionsEnums.h"


Q_DECLARE_LOGGING_CATEGORY(RVOPLog);


class ResultViewOptionsPresenter : public Presenter<ResultViewOptionsView, ResultViewOptionsModel>
{
    Q_OBJECT

public:
    ResultViewOptionsPresenter(QObject* parent = nullptr);
    ~ResultViewOptionsPresenter() = default;

signals:
    void resultViewOptionsChanged();

    //TODO: TMP CODE, REMOVE
    void sortModeChanged(SortMode);
    void filterModeChanged(FilterMode);
    void filterWordsChanged(QString);

    void isUnreviewedVsisibleChanged(bool);
    void isIgnoredVsisibleChanged(bool);
    void isInterestedVsisibleChanged(bool);

private slots:
    void onCurrentSortIndexChanged();

    void onCurrentFilterIndexChanged();
    void onFilterTextChanged();

    void onUnreviewedVisibleChanged();
    void onIgnoredVisibleChanged();
    void onInterestedVisibleChanged();
};