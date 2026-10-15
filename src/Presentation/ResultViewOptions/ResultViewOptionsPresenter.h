#pragma once

#include <Presenter.hpp>

#include "ResultViewOptionsModel.h"
#include "ResultViewOptionsView.h"

#include <QLoggingCategory>

#include <FunctionTypes.h>
#include <VacancyStatus.h>


Q_DECLARE_LOGGING_CATEGORY(RVOPLog);


class ResultViewOptionsPresenter : public Presenter<ResultViewOptionsView, ResultViewOptionsModel>
{
    Q_OBJECT

public:
    ResultViewOptionsPresenter(QObject* parent = nullptr);
    ~ResultViewOptionsPresenter() = default;

public:
    void setFilterMode(int filterMode);
    int filterMode() const;
    const VacancyFilter& vacancyFilter() const;

    void setFilterWords(const QString& filterWords);
    QString filterWords() const;

    void setSortMode(int sortMode);
    int sortMode() const;
    const VacancySortComparator& vacancySortComparator() const;

    void setIsUnreviewedVisible(bool isVisible);
    bool isUnreviewedVisible() const;

    void setIsIgnoredVisible(bool isVisible);
    bool isIgnoredVisible() const;

    void setIsInterestedVisible(bool isVisible);
    bool isInterestedVisible() const;

    QList<VacancyStatus> visibleStatuses() const;

public slots:
    void setKeywords(const QString& keywords);

signals:
    void resultViewOptionsChanged();

    void vacancySortComparatorChanged(const VacancySortComparator& sortComparator);
    void vacancyFilterChanged(const VacancyFilter& filter);

    void visibleStatusesChanged(QList<VacancyStatus> statuses);

private slots:
    void onCurrentSortIndexChanged();

    void onCurrentFilterIndexChanged();
    void onFilterWordsChanged();
    void onFilterWordsWidgetChanged();

    void onUnreviewedVisibleChanged();
    void onIgnoredVisibleChanged();
    void onInterestedVisibleChanged();

private:
    void insertFilters();
    void insertSorts();

    void updateVisibleStatuses();
};

QString filterWordsToString(const QStringList& filterWords);
QStringList filterWordsToList(const QString& filterWords);