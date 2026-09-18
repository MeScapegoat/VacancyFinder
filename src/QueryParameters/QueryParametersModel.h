#pragma once

#include "../MVP/Model.h"

#include <QString>

#include "../utils/Observable.hpp"

class QueryParametersModel : public Model
{
public:
    QueryParametersModel(QObject* parent = nullptr);
    ~QueryParametersModel() = default;

public:
    Observable<QString> keywords;
    Observable<QString> location;
    Observable<int> maxResults;
};