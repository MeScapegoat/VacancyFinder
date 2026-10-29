#pragma once

#include <FunctionTypes.h>
#include <Presenter.hpp>
#include "ResultViewOptionsModel.h"
#include "ResultViewOptionsView.h"
#include <VacancyStatus.h>


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

    void setIsIgnoredVacanciesVisible(bool isVisible);
    bool isIgnoredVacanciesVisible() const;

    void setIsIgnoredCompaniesVisible(bool isVisible);
    bool isIgnoredCompaniesVisible() const;

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
    void ignoredCompaniesVisibilityChanged(bool isVisible);

private slots:
    void onCurrentSortIndexChanged();

    void onCurrentFilterIndexChanged();
    void onFilterWordsChanged();
    void onFilterWordsWidgetChanged();

    void onUnreviewedVisibleChanged();
    void onIgnoredVacanciesVisibleChanged();
    void onIgnoredCompaniesVisibleChanged();
    void onInterestedVisibleChanged();

private:
    void insertFilters();
    void insertSorts();

    void updateVisibleStatuses();
};

QString filterWordsToString(const QStringList& filterWords);
QStringList filterWordsToList(const QString& filterWords);
