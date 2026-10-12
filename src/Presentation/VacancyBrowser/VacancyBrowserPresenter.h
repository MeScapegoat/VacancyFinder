#pragma once

#include <Presenter.hpp>

#include <QLoggingCategory>
#include <QObject>

#include "VacancyBrowserModel.h"
#include "VacancyBrowserView.h"
#include <Vacancy.h>


Q_DECLARE_LOGGING_CATEGORY(VBPLog);


class QModelIndex;

class VacancyBrowserPresenter : public Presenter<VacancyBrowserView, VacancyBrowserModel>
{
    Q_OBJECT

public:
    explicit VacancyBrowserPresenter(QObject* parent = nullptr);
    ~VacancyBrowserPresenter() = default;

public slots:
    void openVacancyDetails(const QModelIndex& index);
    void closeVacancyDetails();

    void onIgnoreButtonPressed();
    void onInterestButtonPressed();
    void onRemoveIgnoreButtonPressed();
    void onRemoveInterestButtonPressed();

    void onBriefReady(const Vacancy& brief);
    void onDetailsReady(const Vacancy& details);

    void updateVacancyList(const QStringList& data);
    void clearVacancyList();

    void updateVisibleVacancies();
    void updateModelData();

private:
    using constVacancyRef = std::reference_wrapper<const Vacancy>;
    std::optional<constVacancyRef> getCurrentVacancy() const;
};