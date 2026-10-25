#include "VacancyBrowserModel.h"

#include <QStringListModel>

VacancyBrowserModel::VacancyBrowserModel(QObject* parent) :
    Model(parent),
    m_vacancyListModel(*new VacancyListModel(this))
{

}

QHash<QUrl, Vacancy>& VacancyBrowserModel::vacancyByUrl()
{
    return m_vacancyByUrl;
}

const QHash<QUrl, Vacancy>& VacancyBrowserModel::vacancyByUrl() const 
{
    return m_vacancyByUrl;
}

QVector<const Vacancy*>& VacancyBrowserModel::visibleVacancies()
{
    return m_visibleVacancies;
}

const QVector<const Vacancy*>& VacancyBrowserModel::visibleVacancies() const
{
    return m_visibleVacancies;
}

QSet<QUrl>& VacancyBrowserModel::ignoredVacancies()
{
    return m_ignoredVacancies;
}

const QSet<QUrl>& VacancyBrowserModel::ignoredVacancies() const
{
    return m_ignoredVacancies;
}

void VacancyBrowserModel::setIgnoredVacancies(const QSet<QUrl>& ignoredVacancies)
{
    m_ignoredVacancies = ignoredVacancies;
}

QSet<QUrl>& VacancyBrowserModel::interestedVacancies()
{
    return m_interestedVacancies;
}

const QSet<QUrl>& VacancyBrowserModel::interestedVacancies() const
{
    return m_interestedVacancies;
}

void VacancyBrowserModel::setInterestedVacancies(const QSet<QUrl>& interestedVacancies)
{
    m_interestedVacancies = interestedVacancies;
}

VacancyBrowserModel::VacancyListModel& VacancyBrowserModel::vacancyListModel()
{
    return m_vacancyListModel;
}

const VacancyBrowserModel::VacancyListModel& VacancyBrowserModel::vacancyListModel() const
{
    return m_vacancyListModel;
}

void VacancyBrowserModel::addVacancy(const Vacancy& vacancy)
{
    const auto it = m_vacancyByUrl.find(vacancy.url);
    if (it == m_vacancyByUrl.end())
    {
        m_vacancyByUrl.emplace(vacancy.url, vacancy);
    }
    else
    {
        Vacancy& foundVacancy = it.value();
        foundVacancy.fillEmptyFrom(vacancy);
    }
}

void VacancyBrowserModel::setVisibleStatuses(QList<VacancyStatus> statuses)
{
    m_visibleStatuses = statuses;
}

void VacancyBrowserModel::setVacancyFilter(const VacancyFilter& filter)
{
    passesWordFilter = filter;
}

void VacancyBrowserModel::setVacancySortComparator(const VacancySortComparator& sortComparator)
{
    sortPredicate = sortComparator;
}

std::optional<constVacancyRef> VacancyBrowserModel::getVisibleVacancy(size_t id) const
{
    if (m_visibleVacancies.size() > id)
    {
        return *m_visibleVacancies[id];
    }

    return std::nullopt;
}

VacancyStatus VacancyBrowserModel::getVacancyStatus(const Vacancy& vacancy) const
{
    const QUrl& url = vacancy.url;

    if (m_interestedVacancies.contains(url))
        return VacancyStatus::Interested;

    if (m_ignoredVacancies.contains(url))
        return VacancyStatus::Ignored;

    return VacancyStatus::Unreviewed;
}

void VacancyBrowserModel::updateVisibleVacancies()
{
    syncVisibleVacancies();
    applyFiltering();
    applySorting();
    updateVacancyListModel();
}

void VacancyBrowserModel::syncVisibleVacancies()
{
    m_visibleVacancies.clear();

    m_visibleVacancies.reserve(m_vacancyByUrl.size());
    for (const Vacancy& vacancy : m_vacancyByUrl)
    {
        m_visibleVacancies.push_back(&vacancy);
    }
}

void VacancyBrowserModel::applyFiltering()
{
    Q_ASSERT_X(passesWordFilter, __FUNCTION__, "passesWordFilter function is not set");

    auto shouldRemoveVacancy = [this](const Vacancy* vacancy) -> bool
    {
        return !passesWordFilter(vacancy) || !passesFilterStatus(vacancy);
    };

    m_visibleVacancies.removeIf(shouldRemoveVacancy);
}

void VacancyBrowserModel::applySorting()
{
    if (sortPredicate)
    {
        std::sort(m_visibleVacancies.begin(), m_visibleVacancies.end(), sortPredicate);
    }
}

void VacancyBrowserModel::updateVacancyListModel()
{
    QStringList data;

    for (const Vacancy* vacancy : m_visibleVacancies)
    {
        const QString& id = QString::number(data.size() + 1);
        const QString& title = vacancy->title;
        const QString& companyName = vacancy->companyName;
        const qsizetype& companyNameWidth = companyName.size() + 8;
        const QString& age = QString::number(vacancy->date.daysTo(QDate::currentDate()));
        const QString& location = vacancy->location;

        data.append(QString("%1. %2\n%3\n%4\n%5 days ago")
                    .arg(id.leftJustified(3))
                    .arg(title)
                    .arg(companyName)
                    .arg(location)
                    .arg(age));
    }

    m_vacancyListModel.setStringList(data);
}

bool VacancyBrowserModel::passesFilterStatus(const Vacancy* vacancy)
{
    const VacancyStatus status = getVacancyStatus(*vacancy);

    return m_visibleStatuses.contains(status);
}
