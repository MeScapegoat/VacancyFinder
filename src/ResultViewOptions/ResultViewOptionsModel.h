#pragma once
#include "../MVP/Model.h"

#include "../utils/Observable.hpp"

#include <QString>

class ResultViewOptionsModel : public Model
{
public:
    ResultViewOptionsModel(QObject* parent = nullptr);
    ~ResultViewOptionsModel() = default;

public:
    Observable<int> currentSortMode;

    Observable<int> currentFilterMode;
    Observable<QString> filterText;

    Observable<bool> isUnreviewedVisible;
    Observable<bool> isIgnoredVisible;
    Observable<bool> isInterestedVisible;
};