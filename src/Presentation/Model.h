#pragma once

#include <QObject>

class Model : public QObject
{
public:
    Model(QObject* parent = nullptr);
    virtual ~Model() = default;
};