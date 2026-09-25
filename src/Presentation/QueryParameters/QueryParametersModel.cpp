#include "QueryParametersModel.h"

QueryParametersModel::QueryParametersModel(QObject* parent) :
    Model(parent),
    keywords(),
    location(),
    maxResults(1)
{
}
