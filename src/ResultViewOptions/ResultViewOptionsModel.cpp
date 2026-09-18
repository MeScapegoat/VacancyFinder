#include "ResultViewOptionsModel.h"

#include "ResultViewOptionsEnums.h"

ResultViewOptionsModel::ResultViewOptionsModel(QObject* parent) :
    Model(parent),
    currentSortMode(static_cast<int>(SortMode::Off)),
    currentFilterMode(static_cast<int>(FilterMode::Off)),
    filterText(),
    isUnreviewedVisible(),
    isIgnoredVisible(),
    isInterestedVisible()
{
}
