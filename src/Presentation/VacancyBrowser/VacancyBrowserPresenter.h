#pragma once

#include <QList>
#include <QUrl>

#include <optional>

#include <FunctionTypes.h>
#include <Presenter.hpp>
#include <Vacancy.h>
#include "VacancyBrowserModel.h"
#include "VacancyBrowserView.h"


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
    void onVacanciesReady(const QList<Vacancy>& vacancies);

    void updateVisibleVacancies();

    void setVisibleStatuses(QList<VacancyStatus>);
    void setVacancyFilter(const VacancyFilter& filter);
    void setVacancySortComparator(const VacancySortComparator& sortComparator);

signals:
    void interestedVacanciesChanged();

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
    std::optional<QUrl> m_currentVacancyUrl;
};
