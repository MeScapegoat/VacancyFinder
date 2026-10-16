#pragma once

#include <Presenter.hpp>

#include <QLoggingCategory>

#include "VacancyBrowserModel.h"
#include "VacancyBrowserView.h"
#include <FunctionTypes.h>
#include <Vacancy.h>


Q_DECLARE_LOGGING_CATEGORY(VBPLog);


class QModelIndex;

class VacancyBrowserPresenter : public Presenter<VacancyBrowserView, VacancyBrowserModel>
{
    Q_OBJECT

public:
    explicit VacancyBrowserPresenter(QObject* parent = nullptr);
    ~VacancyBrowserPresenter() = default;

public:
    const QSet<QUrl>& ignoredVacancies() const;
    void setIgnoredVacancies(const QSet<QUrl>& ignoredVacancies);

    const QSet<QUrl>& interestedVacancies() const;
    void setInterestedVacancies(const QSet<QUrl>& interestedVacancies);

public:
    void reset();

public slots:
    void onBriefReady(const Vacancy& brief);
    void onDetailsReady(const Vacancy& details);

    void updateVisibleVacancies();

    void setVisibleStatuses(QList<VacancyStatus>);
    void setVacancyFilter(const VacancyFilter& filter);
    void setVacancySortComparator(const VacancySortComparator& sortComparator);

private slots:
    void openVacancyDetails(const QModelIndex& index);

    void onIgnoreButtonPressed();
    void onInterestButtonPressed();
    void onRemoveIgnoreButtonPressed();
    void onRemoveInterestButtonPressed();

private:
    bool addCurrentVacancyToStatusList(QSet<QUrl>& statusVacancyList);
    bool removeCurrentVacancyFromStatusList(QSet<QUrl>& statusVacancyList);
    void updateStatusButtons(VacancyStatus status);

private:
    std::optional<constVacancyRef> getCurrentVacancy() const;
};