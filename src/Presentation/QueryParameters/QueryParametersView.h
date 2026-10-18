#pragma once

#include <View.h>

#include <QWidget>
#include <QObject>

class QLineEdit;
class QSpinBox;
class QPushButton;

class QueryParametersView : public View
{
public:
    QueryParametersView(QWidget* parent = nullptr);
    ~QueryParametersView() = default;

public:
    QLineEdit& keywordsInput;
    QLineEdit& locationInput;
    QSpinBox& maxResultsInput;

    QPushButton& searchButton;
    QPushButton& searchInterestedButton;
    QPushButton& resetButton;
};