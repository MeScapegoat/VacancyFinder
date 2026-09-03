#include "AppState.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QFileInfo>


Q_LOGGING_CATEGORY(StateLog, "state");


namespace
{
    inline constexpr const char* StateFileName = "app_state.json";

    inline constexpr const char* InterestedKey = "interested";
    inline constexpr const char* IgnoredKey = "ignored";

    inline constexpr const char* KeywordsKey = "keywords";
    inline constexpr const char* LocationKey = "location";

    inline constexpr const char* LimitKey = "limit";

    inline constexpr const char* FilterModeKey = "filterMode";
    inline constexpr const char* FiltersKey = "filters";

    inline constexpr const char* SortModeKey = "sortMode";

    inline constexpr const char* IsUnreviewedVisibleKey = "isUnreviewedVisible";
    inline constexpr const char* IsIgnoredVisibleKey = "isIgnoredVisible";
    inline constexpr const char* IsInterestedVisibleKey = "isInterestedVisible";
}


AppState::AppState() :
    interested(),
    ignored(),
    keywords(),
    location(),
    filterMode(0),
    filters(),
    sortMode(0),
    isUnreviewedVisible(false),
    isIgnoredVisible(false),
    isInterestedVisible(false)
{
    qCDebug(StateLog) << __FUNCTION__;
}

AppState::~AppState()
{
    qCDebug(StateLog) << __FUNCTION__;
}

const QSet<QUrl> AppState::getIgnored() const
{
    return ignored;
}

void AppState::setIgnored(const QSet<QUrl> _ignored)
{
    ignored = _ignored;
}

const QSet<QUrl> AppState::getInterested() const
{
    return interested;
}

void AppState::setInterested(const QSet<QUrl>& _interested)
{
    interested = _interested;
}

const QString& AppState::getKeywords() const
{
    return keywords;
}

void AppState::setKeywords(const QString& _keywords)
{
    keywords = _keywords;
}

const QString& AppState::getLocation() const
{
    return location;
}

void AppState::setLocation(const QString& _location)
{
    location = _location;
}

const int AppState::getLimit() const
{
    return limit;
}

void AppState::setLimit(const int _limit)
{
    limit = _limit;
}

const int AppState::getFilterMode() const
{
    return filterMode;
}

void AppState::setFilterMode(const int _filterMode)
{
    filterMode = _filterMode;
}

const QString& AppState::getFilters() const
{
    return filters;
}

void AppState::setFilters(const QString& _filters)
{
    filters = _filters;
}

const int AppState::getSortMode() const
{
    return sortMode;
}

void AppState::setSortMode(const int _sortMode)
{
    sortMode = _sortMode;
}

const bool AppState::getIsUnreviewedVisible() const
{
    return isUnreviewedVisible;
}

void AppState::setIsUnreviewedVisible(const bool _isUnreviewedVisible)
{
    isUnreviewedVisible = _isUnreviewedVisible;
}

const bool AppState::getIsIgnoredVisible() const
{
    return isIgnoredVisible;
}

void AppState::setIsIgnoredVisible(const bool _isIgnoredVisible)
{
    isIgnoredVisible = _isIgnoredVisible;
}

const bool AppState::getIsInterestedVisible() const
{
    return isInterestedVisible;
}

void AppState::setIsInterestedVisible(const bool _isInterestedVisible)
{
    isInterestedVisible = _isInterestedVisible;
}

void AppState::saveState()
{
    qCDebug(StateLog) << __FUNCTION__;
    qCDebug(StateLog) << *this;

    QJsonObject state;
    state[InterestedKey] = QJsonArray::fromStringList(toStringList(interested));
    state[IgnoredKey] = QJsonArray::fromStringList(toStringList(ignored));

    state[KeywordsKey] = keywords;
    state[LocationKey] = location;

    state[LimitKey] = limit;

    state[FilterModeKey] = filterMode;
    state[FiltersKey] = filters;

    state[SortModeKey] = sortMode;

    state[IsUnreviewedVisibleKey] = isUnreviewedVisible;
    state[IsIgnoredVisibleKey] = isIgnoredVisible;
    state[IsInterestedVisibleKey] = isInterestedVisible;

    QFile file(StateFileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qCWarning(StateLog) << "Couldn't open file:" << QFileInfo(file).absoluteFilePath();
        return;
    }

    file.write(QJsonDocument(state).toJson(QJsonDocument::Indented));
}

void AppState::loadState()
{
    qCDebug(StateLog) << __FUNCTION__;

    QFile file(StateFileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        qCWarning(StateLog) << "Couldn't open file:" << QFileInfo(file).absoluteFilePath();
        return;
    }

    const QJsonDocument document = QJsonDocument::fromJson(file.readAll());
    if (!document.isObject())
    {
        qCWarning(StateLog) << "Incorrect json format:" << QFileInfo(file).absoluteFilePath();
        return;
    }

    const QJsonObject state = document.object();

    interested = toUrlSet(state[InterestedKey].toVariant().toStringList());
    ignored = toUrlSet(state[IgnoredKey].toVariant().toStringList());

    keywords = state[KeywordsKey].toString();
    location = state[LocationKey].toString();

    limit = state[LimitKey].toInt();

    filterMode = state[FilterModeKey].toInt();
    filters = state[FiltersKey].toString();

    sortMode = state[SortModeKey].toInt();

    isUnreviewedVisible = state[IsUnreviewedVisibleKey].toBool();
    isIgnoredVisible = state[IsIgnoredVisibleKey].toBool();
    isInterestedVisible = state[IsInterestedVisibleKey].toBool();

    qCDebug(StateLog) << *this;
}

QStringList AppState::toStringList(const QSet<QUrl>& set)
{
    QStringList result;

    for (const QUrl& url : set)
    {
        result.append(url.toString());
    }

    return result;
}

QSet<QUrl> AppState::toUrlSet(const QStringList& stringList)
{
    QSet<QUrl> result;

    for (const QString& string : stringList)
    {
        result.insert(string);
    }

    return result;
}

QDebug operator<<(QDebug debug, const AppState& appState)
{
        QDebugStateSaver saver(debug);

        debug.nospace() << "AppState:" << "\n"
            << "interested=          " << appState.getInterested() << "\n"
            << "ignored=             " << appState.getIgnored() << "\n"
            << "keywords=            " << appState.getKeywords() << "\n"
            << "location=            " << appState.getLocation() << "\n"
            << "limit=               " << appState.getLimit() << "\n"
            << "filterMode=          " << appState.getFilterMode() << "\n"
            << "filters=             " << appState.getFilters() << "\n"
            << "sortMode=            " << appState.getSortMode() << "\n"
            << "isUnreviewedVisible= " << appState.getIsUnreviewedVisible() << "\n"
            << "isIgnoredVisible=    " << appState.getIsIgnoredVisible() << "\n"
            << "isInterestedVisible= " << appState.getIsInterestedVisible();

        return debug;
}
