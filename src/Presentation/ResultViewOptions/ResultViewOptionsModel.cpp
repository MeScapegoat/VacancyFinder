#include "ResultViewOptionsModel.h"

#include "ResultViewOptionsEnums.h"

ResultViewOptionsModel::ResultViewOptionsModel(QObject* parent) :
    Model(parent),
    currentSortMode(static_cast<int>(SortMode::Off)),
    currentFilterMode(static_cast<int>(FilterMode::Off)),
    filterWordList(),
    isUnreviewedVisible(true),
    isIgnoredVacanciesVisible(true),
    isIgnoredCompaniesVisible(true),
    isInterestedVisible(true)
{
}

void ResultViewOptionsModel::setKeywords(const QString& keywords)
{
    m_keywords = keywords;
}

const QString& ResultViewOptionsModel::keywords() const
{
    return m_keywords;
}
