#include "VacancyBrowserModel.h"

#include <QStringListModel>

VacancyBrowserModel::VacancyBrowserModel(QObject* parent) :
    Model(parent),
    m_vacancyListModel(*new QStringListModel)
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

QStringListModel& VacancyBrowserModel::vacancyListModel()
{
    return m_vacancyListModel;
}

const QStringListModel& VacancyBrowserModel::vacancyListModel() const 
{
    return m_vacancyListModel;
}

const Vacancy* const VacancyBrowserModel::getVisibleVacancy(size_t id) const
{
    if (id < m_visibleVacancies.size())
    {
        return m_visibleVacancies[id];
    }

    return nullptr;
}

VacancyStatus VacancyBrowserModel::getVacancyStatus(const Vacancy& vacancy) const
{
    const QUrl& url = vacancy.link;

    if (m_interestedVacancies.contains(url))
        return VacancyStatus::Interested;

    if (m_ignoredVacancies.contains(url))
        return VacancyStatus::Ignored;

    return VacancyStatus::Unreviewed;
}

void VacancyBrowserModel::updateVisibleVacancies()
{
    syncVisibleVacancies();

    applyStatusFilter();

    applyFiltering();

    applySorting();
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

void VacancyBrowserModel::applyStatusFilter()
{
    // TODO: std::erase_if
    for (auto it = m_visibleVacancies.begin(); it != m_visibleVacancies.end(); /* EMPTY */)
    {
        const Vacancy& vacancy = **it;
        const VacancyStatus status = getVacancyStatus(vacancy);
        bool isRemoving = false;

        switch (status)
        {
            case VacancyStatus::Ignored:
            {
                isRemoving = !m_isIgnoredVisible;
                break;
            }
            case VacancyStatus::Interested:
            {
                isRemoving = !m_isInterestedVisible;
                break;
            }
            case VacancyStatus::Unreviewed:
            {
                isRemoving = !m_isUnreviewedVisible;
                break;
            }
        }

        if (isRemoving)
            it = m_visibleVacancies.erase(it);
        else
            ++it;
    }
}

void VacancyBrowserModel::applyFiltering()
{
    QStringList filterWords = m_filterWords.split(',');
    if (filterWords.isEmpty()) return;

    for (QString& word : filterWords)
        word = word.trimmed();

    switch (m_filterMode)
    {
        case FilterMode::Off:
        {
            return;
        }
        case FilterMode::Include:
        {
            // TODO: std::erase_if
            //qCDebug(AppLog) << "Include filter mode, filter words:" << filterWords;
            for (auto it = m_visibleVacancies.begin(); it != m_visibleVacancies.end(); /* EMPTY */)
            {
                const Vacancy& vacancy = **it;
                bool isFilterOk = true;
                for (const QString& word : filterWords)
                {
                    if (vacancy.count(word) == 0)
                    {
                        isFilterOk = false;
                        break;
                    }
                }

                if (!isFilterOk)
                    it = m_visibleVacancies.erase(it);
                else
                    ++it;
            }
            break;
        }
        case FilterMode::Exclude:
        {
            // TODO: std::erase_if
            //qCDebug(AppLog) << "Exclude filter mode, filter words:" << filterWords;
            for (auto it = m_visibleVacancies.begin(); it != m_visibleVacancies.end(); /* EMPTY */)
            {
                const Vacancy& vacancy = **it;
                bool isFilterOk = true;
                for (const QString& word : filterWords)
                {
                    if (vacancy.count(word) != 0)
                    {
                        isFilterOk = false;
                        break;
                    }
                }

                if (!isFilterOk)
                    it = m_visibleVacancies.erase(it);
                else
                    ++it;
            }
            break;
        }
    }
}

void VacancyBrowserModel::applySorting()
{
    switch (m_sortMode)
    {
        case SortMode::Off:
        {
            return;
        }
        case SortMode::Relevance:
        {
            sortByRelevance();
            break;
        }
        case SortMode::Title:
        {
            sortByTitle();
            break;
        }
        case SortMode::Company:
        {
            sortByCompany();
            break;
        }
        case SortMode::Time:
        {
            sortByTime();
            break;
        }
        case SortMode::EmploymentType:
        {
            sortByEmploymentType();
            break;
        }
        case SortMode::Level:
        {
            sortByLevel();
            break;
        }
        case SortMode::Experience:
        {
            sortByExperience();
            break;
        }
        case SortMode::Education:
        {
            sortByEducation();
            break;
        }
        case SortMode::Skills:
        {
            sortBySkills();
            break;
        }
        case SortMode::Industry:
        {
            sortByIndustry();
            break;
        }
        case SortMode::ExpirationDate:
        {
            sortByExpirationDate();
            break;
        }
    }
}

void VacancyBrowserModel::sortByRelevance()
{
    //qCDebug(AppLog) << "Sorting by relevance";

    auto compareByRelevance = [&](const Vacancy* left, const Vacancy* right)
    {
        return left->count(m_keywords) > right->count(m_keywords);
    };

    std::sort(m_visibleVacancies.begin(), m_visibleVacancies.end(), compareByRelevance);
}

void VacancyBrowserModel::sortByTitle()
{
    //qCDebug(AppLog) << "Sorting by title";

    auto compareByTitle = [](const Vacancy* left, const Vacancy* right)
    {
        return left->title < right->title;
    };

    std::sort(m_visibleVacancies.begin(), m_visibleVacancies.end(), compareByTitle);
}

void VacancyBrowserModel::sortByCompany()
{
    //qCDebug(AppLog) << "Sorting by company";

    auto compareByCompany = [](const Vacancy* left, const Vacancy* right)
    {
        return left->companyName < right->companyName;
    };

    std::sort(m_visibleVacancies.begin(), m_visibleVacancies.end(), compareByCompany);
}

void VacancyBrowserModel::sortByTime()
{
    //qCDebug(AppLog) << "Sorting by time";

    auto compareByTime = [](const Vacancy* left, const Vacancy* right)
    {
        return left->date > right->date;
    };

    std::sort(m_visibleVacancies.begin(), m_visibleVacancies.end(), compareByTime);
}

void VacancyBrowserModel::sortByEmploymentType()
{
    //qCDebug(AppLog) << "sorting by Employment Type";
    auto compareByEmploymentType = [](const Vacancy* left, const Vacancy* right)
    {
        return left->employmentType > right->employmentType;
    };

    std::sort(m_visibleVacancies.begin(), m_visibleVacancies.end(), compareByEmploymentType);
}

void VacancyBrowserModel::sortByLevel()
{
    //qCDebug(AppLog) << "Sorting by level";

    auto compareByLevel = [&](const Vacancy* left, const Vacancy* right)
    {
        if (!left->level.has_value()) return false;
        if (!right->level.has_value()) return true;

        return left->level < right->level;
    };

    std::sort(m_visibleVacancies.begin(), m_visibleVacancies.end(), compareByLevel);
}

void VacancyBrowserModel::sortByExperience()
{
    //qCDebug(AppLog) << "Sorting by experience";

    auto compareByExperience = [&](const Vacancy* left, const Vacancy* right)
    {
        if (!left->experience.has_value()) return false;
        if (!right->experience.has_value()) return true;

        return left->experience < right->experience;
    };

    std::sort(m_visibleVacancies.begin(), m_visibleVacancies.end(), compareByExperience);
}

void VacancyBrowserModel::sortByEducation()
{
    //qCDebug(AppLog) << "Sorting by education";

    auto compareByEducation = [&](const Vacancy* left, const Vacancy* right)
    {
        if (!left->education.has_value()) return false;
        if (!right->education.has_value()) return true;

        return left->education < right->education;
    };

    std::sort(m_visibleVacancies.begin(), m_visibleVacancies.end(), compareByEducation);
}

void VacancyBrowserModel::sortBySkills()
{
    //qCDebug(AppLog) << "Sorting by skills";

    auto compareBySkills = [&](const Vacancy* left, const Vacancy* right)
    {
        if (left->skills.isEmpty()) return false;
        if (right->skills.isEmpty()) return true;

        return left->skills < right->skills;
    };

    std::sort(m_visibleVacancies.begin(), m_visibleVacancies.end(), compareBySkills);
}

void VacancyBrowserModel::sortByIndustry()
{
    //qCDebug(AppLog) << "sorting by industry";

    auto compareByIndustry = [&](const Vacancy* left, const Vacancy* right)
    {
        if (!left->industry.has_value()) return false;
        if (!right->industry.has_value()) return true;

        return left->industry < right->industry;
    };

    std::sort(m_visibleVacancies.begin(), m_visibleVacancies.end(), compareByIndustry);
}

void VacancyBrowserModel::sortByExpirationDate()
{
    //qCDebug(AppLog) << "sorting by Relevance";

    auto compareByExpirationDate = [&](const Vacancy* left, const Vacancy* right)
    {
        if (!left->expirationDate.has_value()) return false;
        if (!right->expirationDate.has_value()) return true;

        return left->expirationDate < right->expirationDate;
    };

    std::sort(m_visibleVacancies.begin(), m_visibleVacancies.end(), compareByExpirationDate);
}