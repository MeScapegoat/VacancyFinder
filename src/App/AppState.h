#pragma once

#include <QLoggingCategory>
#include <QSet>
#include <QUrl>
#include <QString>
#include <QStringList>


Q_DECLARE_LOGGING_CATEGORY(StateLog);


class AppState
{
public:
    AppState();
    ~AppState();

public:
    const QSet<QUrl> getIgnored() const;
    void setIgnored(const QSet<QUrl> ignored);

    const QSet<QUrl> getInterested() const;
    void setInterested(const QSet<QUrl>& interested);


    const QString& getKeywords() const;
    void setKeywords(const QString& keywords);

    const QString& getLocation() const;
    void setLocation(const QString& location);


    const int getLimit() const;
    void setLimit(const int limit);


    const int getFilterMode() const;
    void setFilterMode(const int filterMode);

    const QString& getFilters() const;
    void setFilters(const QString& filters);


    const int getSortMode() const;
    void setSortMode(const int sortMode);


    const bool getIsUnreviewedVisible() const;
    void setIsUnreviewedVisible(const bool isUnreviewedVisible);

    const bool getIsIgnoredVisible() const;
    void setIsIgnoredVisible(const bool isIgnoredVisible);

    const bool getIsInterestedVisible() const;
    void setIsInterestedVisible(const bool isInterestedVisible);

public:
    void saveState();

    void loadState();

protected:
    QStringList toStringList(const QSet<QUrl>& qset);
    QSet<QUrl> toUrlSet(const QStringList& stringList);

protected:
    QSet<QUrl> ignored;
    QSet<QUrl> interested;

    QString keywords;
    QString location;

    int limit;

    int filterMode; // TODO: make enum
    QString filters;

    int sortMode; // TODO: make enum

    bool isUnreviewedVisible;
    bool isIgnoredVisible;
    bool isInterestedVisible;
};

QDebug operator<<(QDebug debug, const AppState& appState);