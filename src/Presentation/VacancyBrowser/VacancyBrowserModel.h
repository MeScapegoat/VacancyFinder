#pragma once

#include <Model.h>

#include <QHash>
#include <QSet>
#include <QString>
#include <QUrl>
#include <QVector>

#include "VacancyBrowserEnums.h"
#include <Vacancy.h>
//#include <Observable.hpp>
#include <ResultViewOptions/ResultViewOptionsEnums.h>

class QStringListModel;

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

    QStringListModel& vacancyListModel();
    const QStringListModel& vacancyListModel() const;

public:
    const Vacancy* const getVisibleVacancy(size_t id) const;
    VacancyStatus getVacancyStatus(const Vacancy& vacancy) const;

public:
    void updateVisibleVacancies();

private:
    QHash<QUrl, Vacancy> m_vacancyByUrl;
    QVector<const Vacancy*> m_visibleVacancies;

    QSet<QUrl> m_ignoredVacancies;
    QSet<QUrl> m_interestedVacancies;

    QStringListModel& m_vacancyListModel;

    // TODO: TMP CODE, REMOVE
    void syncVisibleVacancies();
    void applyStatusFilter();
    void applyFiltering();
    void applySorting();

    void sortByRelevance();
    void sortByTitle();
    void sortByCompany();
    void sortByTime();
    void sortByEmploymentType();
    void sortByLevel();
    void sortByExperience();
    void sortByEducation();
    void sortBySkills();
    void sortByIndustry();
    void sortByExpirationDate();

    SortMode m_sortMode;
    FilterMode m_filterMode;
    bool m_isUnreviewedVisible = false;
    bool m_isIgnoredVisible = false;
    bool m_isInterestedVisible = false;
    QString m_keywords;
    QString m_filterWords;

public slots:
    void setSortMode(SortMode sortMode) {m_sortMode = sortMode;};
    void setFilterMode(FilterMode filterMode) {m_filterMode = filterMode;};
    void setIsUnreviewedVisible(bool isUnreviewedVisible) {m_isUnreviewedVisible = isUnreviewedVisible;};
    void setIsIgnoredVisible(bool isIgnoredVisible) {m_isIgnoredVisible = isIgnoredVisible;};
    void setIsInterestedVisible(bool isInterestedVisible) {m_isInterestedVisible = isInterestedVisible;};
    void setKeywords(QString keywords) {m_keywords = keywords;};
    void setFilterWords(QString filterWords) {m_filterWords = filterWords;};
};