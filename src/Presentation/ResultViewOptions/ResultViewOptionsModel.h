#pragma once
#include <Model.h>

#include <Observable.hpp>

#include <QString>

class ResultViewOptionsModel : public Model
{
public:
    ResultViewOptionsModel(QObject* parent = nullptr);
    ~ResultViewOptionsModel() = default;

public:
    void setKeywords(const QString& keywords);
    const QString& keywords() const;

public:
    Observable<int> currentSortMode;

    Observable<int> currentFilterMode;
    Observable<QStringList> filterWordList;

    Observable<bool> isUnreviewedVisible;
    Observable<bool> isIgnoredVacanciesVisible;
    Observable<bool> isIgnoredCompaniesVisible;
    Observable<bool> isInterestedVisible;

private:
    QString m_keywords;
};
