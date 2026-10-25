#include "AppState.h"

#include <QByteArray>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QLoggingCategory>

#include <Utils.hpp>


Q_LOGGING_CATEGORY(StateLog, "state");


namespace
{
    inline constexpr const char* StateFileName = "app_state.json";

    inline constexpr const char* InterestedVacanciesKey = "interestedVacancies";
    inline constexpr const char* IgnoredVacanciesKey = "ignoredVacancies";

    inline constexpr const char* KeywordsKey = "keywords";
    inline constexpr const char* LocationKey = "location";

    inline constexpr const char* MaxResultsKey = "maxResults";

    inline constexpr const char* FilterModeKey = "filterMode";
    inline constexpr const char* FiltersKey = "filters";

    inline constexpr const char* SortModeKey = "sortMode";

    inline constexpr const char* IsUnreviewedVisibleKey = "isUnreviewedVisible";
    inline constexpr const char* IsIgnoredVisibleKey = "isIgnoredVisible";
    inline constexpr const char* IsInterestedVisibleKey = "isInterestedVisible";
}


AppState::AppState() :
    m_ignoredVacancies(),
    m_interestedVacancies(),
    m_keywords(),
    m_location(),
    m_maxResults(1),
    m_filterMode(0),
    m_filters(),
    m_sortMode(0),
    m_isUnreviewedVisible(false),
    m_isIgnoredVisible(false),
    m_isInterestedVisible(false)
{
}

const QSet<QUrl> AppState::ignoredVacancies() const
{
    return m_ignoredVacancies;
}

void AppState::setIgnoredVacancies(const QSet<QUrl> ignoredVacancies)
{
    m_ignoredVacancies = ignoredVacancies;
}

const QSet<QUrl> AppState::interestedVacancies() const
{
    return m_interestedVacancies;
}

void AppState::setInterestedVacancies(const QSet<QUrl>& interestedVacancies)
{
    m_interestedVacancies = interestedVacancies;
}

const QString& AppState::keywords() const
{
    return m_keywords;
}

void AppState::setKeywords(const QString& _keywords)
{
    m_keywords = _keywords;
}

const QString& AppState::location() const
{
    return m_location;
}

void AppState::setLocation(const QString& _location)
{
    m_location = _location;
}

int AppState::maxResults() const
{
    return m_maxResults;
}

void AppState::setMaxResults(int maxResults)
{
    m_maxResults = maxResults;
}

const int AppState::filterMode() const
{
    return m_filterMode;
}

void AppState::setFilterMode(const int _filterMode)
{
    m_filterMode = _filterMode;
}

const QString& AppState::filters() const
{
    return m_filters;
}

void AppState::setFilters(const QString& _filters)
{
    m_filters = _filters;
}

const int AppState::sortMode() const
{
    return m_sortMode;
}

void AppState::setSortMode(const int _sortMode)
{
    m_sortMode = _sortMode;
}

const bool AppState::isUnreviewedVisible() const
{
    return m_isUnreviewedVisible;
}

void AppState::setIsUnreviewedVisible(const bool _isUnreviewedVisible)
{
    m_isUnreviewedVisible = _isUnreviewedVisible;
}

const bool AppState::isIgnoredVisible() const
{
    return m_isIgnoredVisible;
}

void AppState::setIsIgnoredVisible(const bool _isIgnoredVisible)
{
    m_isIgnoredVisible = _isIgnoredVisible;
}

const bool AppState::isInterestedVisible() const
{
    return m_isInterestedVisible;
}

void AppState::setIsInterestedVisible(const bool _isInterestedVisible)
{
    m_isInterestedVisible = _isInterestedVisible;
}

void AppState::saveState()
{
    QJsonObject state;
    state[InterestedVacanciesKey] = QJsonArray::fromStringList(toStringList(m_interestedVacancies));
    state[IgnoredVacanciesKey] = QJsonArray::fromStringList(toStringList(m_ignoredVacancies));

    state[KeywordsKey] = m_keywords;
    state[LocationKey] = m_location;

    state[MaxResultsKey] = m_maxResults;

    state[FilterModeKey] = m_filterMode;
    state[FiltersKey] = m_filters;

    state[SortModeKey] = m_sortMode;

    state[IsUnreviewedVisibleKey] = m_isUnreviewedVisible;
    state[IsIgnoredVisibleKey] = m_isIgnoredVisible;
    state[IsInterestedVisibleKey] = m_isInterestedVisible;

    QFile file(StateFileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qCWarning(StateLog) << "Could not open state file for writing:" << QFileInfo(file).absoluteFilePath() << file.errorString();
        return;
    }

    const QByteArray json = QJsonDocument(state).toJson(QJsonDocument::Indented);
    if (file.write(json) != json.size())
        qCWarning(StateLog) << "Could not write state file:" << QFileInfo(file).absoluteFilePath() << file.errorString();
}

void AppState::loadState()
{
    QFile file(StateFileName);
    if (!file.exists())
        return;

    if (!file.open(QIODevice::ReadOnly))
    {
        qCWarning(StateLog) << "Could not open state file for reading:" << QFileInfo(file).absoluteFilePath() << file.errorString();
        return;
    }

    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &parseError);
    if (parseError.error != QJsonParseError::NoError)
    {
        qCWarning(StateLog) << "Could not parse state file:" << QFileInfo(file).absoluteFilePath() << parseError.errorString();
        return;
    }

    if (!document.isObject())
    {
        qCWarning(StateLog) << "State file must contain a JSON object:" << QFileInfo(file).absoluteFilePath();
        return;
    }

    const QJsonObject state = document.object();

    m_interestedVacancies = toUrlSet(state[InterestedVacanciesKey].toVariant().toStringList());
    m_ignoredVacancies = toUrlSet(state[IgnoredVacanciesKey].toVariant().toStringList());

    m_keywords = state[KeywordsKey].toString();
    m_location = state[LocationKey].toString();

    m_maxResults = state[MaxResultsKey].toInt();

    m_filterMode = state[FilterModeKey].toInt();
    m_filters = state[FiltersKey].toString();

    m_sortMode = state[SortModeKey].toInt();

    m_isUnreviewedVisible = state[IsUnreviewedVisibleKey].toBool();
    m_isIgnoredVisible = state[IsIgnoredVisibleKey].toBool();
    m_isInterestedVisible = state[IsInterestedVisibleKey].toBool();
}
