#pragma once

#include <QHash>
#include <QList>
#include <QSet>
#include <QUrl>
#include <QVector>

#include <functional>
#include <optional>

#include <FunctionTypes.h>
#include <Model.h>
#include <Vacancy.h>
#include <VacancyListModel.h>
#include <VacancyStatus.h>

using constVacancyRef = std::reference_wrapper<const Vacancy>;

class VacancyBrowserModel : public Model
{
public:
    VacancyBrowserModel(QObject* parent = nullptr);
    ~VacancyBrowserModel() = default;

public:
    QHash<QUrl, Vacancy>& vacancyByUrl();
    const QHash<QUrl, Vacancy>& vacancyByUrl() const;

    QVector<const Vacancy*>& visibleVacancies();
    const QVector<const Vacancy*>& visibleVacancies() const;

    QSet<QUrl>& ignoredVacancies();
    const QSet<QUrl>& ignoredVacancies() const;
    void setIgnoredVacancies(const QSet<QUrl>& ignoredVacancies);

    QSet<QUrl>& interestedVacancies();
    const QSet<QUrl>& interestedVacancies() const;
    void setInterestedVacancies(const QSet<QUrl>& interestedVacancies);

    VacancyListModel& vacancyListModel();
    const VacancyListModel& vacancyListModel() const;

public:
    void addVacancy(const Vacancy& vacancy);

    void setVisibleStatuses(QList<VacancyStatus> statuses);
    void setVacancyFilter(const VacancyFilter& filter);
    void setVacancySortComparator(const VacancySortComparator& sortComparator);

public:
    std::optional<constVacancyRef> getVisibleVacancy(size_t id) const;
    VacancyStatus getVacancyStatus(const Vacancy& vacancy) const;

public:
    void updateVisibleVacancies();

private:
    void syncVisibleVacancies();
    void applyFiltering();
    void applySorting();
    void updateVacancyListModel();

private:
    bool passesFilterStatus(const Vacancy* vacancy);

private:
    QHash<QUrl, Vacancy> m_vacancyByUrl;
    QVector<const Vacancy*> m_visibleVacancies;

    QSet<QUrl> m_ignoredVacancies;
    QSet<QUrl> m_interestedVacancies;

    VacancyListModel& m_vacancyListModel;

    QList<VacancyStatus> m_visibleStatuses;
    VacancyFilter passesWordFilter;

    VacancySortComparator sortPredicate;
};
