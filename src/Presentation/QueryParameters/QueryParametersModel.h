#pragma once

#include <Model.h>

#include <QString>

#include <Observable.hpp>

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