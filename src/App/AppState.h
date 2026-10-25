#pragma once

#include <QLoggingCategory>
#include <QSet>
#include <QString>
#include <QUrl>


Q_DECLARE_LOGGING_CATEGORY(StateLog);


class AppState
{
public:
    AppState();

public:
    const QSet<QUrl> ignoredVacancies() const;
    void setIgnoredVacancies(const QSet<QUrl> ignoredVacancies);

    const QSet<QUrl> interestedVacancies() const;
    void setInterestedVacancies(const QSet<QUrl>& interestedVacancies);


    const QString& keywords() const;
    void setKeywords(const QString& keywords);

    const QString& location() const;
    void setLocation(const QString& location);


    int maxResults() const;
    void setMaxResults(int maxResults);


    const int filterMode() const;
    void setFilterMode(const int filterMode);

    const QString& filters() const;
    void setFilters(const QString& filters);


    const int sortMode() const;
    void setSortMode(const int sortMode);


    const bool isUnreviewedVisible() const;
    void setIsUnreviewedVisible(const bool isUnreviewedVisible);

    const bool isIgnoredVisible() const;
    void setIsIgnoredVisible(const bool isIgnoredVisible);

    const bool isInterestedVisible() const;
    void setIsInterestedVisible(const bool isInterestedVisible);

public:
    void saveState();

    void loadState();

protected:
    QSet<QUrl> m_ignoredVacancies;
    QSet<QUrl> m_interestedVacancies;

    QString m_keywords;
    QString m_location;

    int m_maxResults;

    int m_filterMode;
    QString m_filters;

    int m_sortMode;

    bool m_isUnreviewedVisible;
    bool m_isIgnoredVisible;
    bool m_isInterestedVisible;
};
