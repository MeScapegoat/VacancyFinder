#include "ResultViewOptionsPresenter.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QLineEdit>


Q_LOGGING_CATEGORY(RVOPLog, "resultViewOptions.presenter");


ResultViewOptionsPresenter::ResultViewOptionsPresenter(QObject* parent) : 
    Presenter(parent)
{
    qCDebug(RVOPLog) << __FUNCTION__;

    const ResultViewOptionsModel& model = getModel();
    const ResultViewOptionsView& view = getView();

    connect(&view.sortMode, &QComboBox::currentIndexChanged, &model.currentSortMode, &Observable<int>::setValue);
    connect(&model.currentSortMode, &Observable<int>::valueChanged, this, &ResultViewOptionsPresenter::onCurrentSortIndexChanged);


    connect(&view.filterMode, &QComboBox::currentIndexChanged, &model.currentFilterMode, &Observable<int>::setValue);
    connect(&model.currentFilterMode, &Observable<int>::valueChanged, this, &ResultViewOptionsPresenter::onCurrentFilterIndexChanged);

    connect(&view.filterLineEdit, &QLineEdit::textEdited, &model.filterText, &Observable<QString>::setValue);
    connect(&model.filterText, &Observable<QString>::valueChanged, this, &ResultViewOptionsPresenter::onFilterTextChanged);


    connect(&view.unreviewedCheckBox, &QCheckBox::checkStateChanged, &model.isUnreviewedVisible, &Observable<bool>::setValue);
    connect(&model.isUnreviewedVisible, &Observable<bool>::valueChanged, this, &ResultViewOptionsPresenter::onUnreviewedVisibleChanged);

    connect(&view.ignoredCheckBox, &QCheckBox::checkStateChanged, &model.isIgnoredVisible, &Observable<bool>::setValue);
    connect(&model.isIgnoredVisible, &Observable<bool>::valueChanged, this, &ResultViewOptionsPresenter::onIgnoredVisibleChanged);

    connect(&view.interestedCheckBox, &QCheckBox::checkStateChanged, &model.isInterestedVisible, &Observable<bool>::setValue);
    connect(&model.isInterestedVisible, &Observable<bool>::valueChanged, this, &ResultViewOptionsPresenter::onInterestedVisibleChanged);
}

void ResultViewOptionsPresenter::onCurrentSortIndexChanged()
{
    qCDebug(RVOPLog) << __FUNCTION__;

    const ResultViewOptionsModel& model = getModel();
    const ResultViewOptionsView& view = getView();

    view.sortMode.setCurrentIndex(model.currentSortMode());
}

void ResultViewOptionsPresenter::onCurrentFilterIndexChanged()
{
    qCDebug(RVOPLog) << __FUNCTION__;

    const ResultViewOptionsModel& model = getModel();
    const ResultViewOptionsView& view = getView();

    view.filterMode.setCurrentIndex(model.currentFilterMode());
}

void ResultViewOptionsPresenter::onFilterTextChanged()
{
    qCDebug(RVOPLog) << __FUNCTION__;

    const ResultViewOptionsModel& model = getModel();
    const ResultViewOptionsView& view = getView();

    view.filterLineEdit.setText(model.filterText());
}

void ResultViewOptionsPresenter::onUnreviewedVisibleChanged()
{
    qCDebug(RVOPLog) << __FUNCTION__;

    const ResultViewOptionsModel& model = getModel();
    const ResultViewOptionsView& view = getView();

    view.unreviewedCheckBox.setChecked(model.isUnreviewedVisible());
}

void ResultViewOptionsPresenter::onIgnoredVisibleChanged()
{
    qCDebug(RVOPLog) << __FUNCTION__;

    const ResultViewOptionsModel& model = getModel();
    const ResultViewOptionsView& view = getView();

    view.ignoredCheckBox.setChecked(model.isIgnoredVisible());
}

void ResultViewOptionsPresenter::onInterestedVisibleChanged()
{
    qCDebug(RVOPLog) << __FUNCTION__;

    const ResultViewOptionsModel& model = getModel();
    const ResultViewOptionsView& view = getView();

    view.interestedCheckBox.setChecked(model.isInterestedVisible());
}
