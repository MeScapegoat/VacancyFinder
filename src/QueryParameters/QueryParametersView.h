#pragma once

#include "../MVP/View.h"

#include <QWidget>
#include <QObject>

class QLineEdit;
class QSpinBox;
class QPushButton;

class QueryParametersView : public QWidget, public View
{
public:
    QueryParametersView(QWidget* parent = nullptr);
    ~QueryParametersView() = default;

public:
    // View
    QWidget& widget() override;
    const QWidget& widget() const override;

public:
    QLineEdit& keywordsInput;
    QLineEdit& locationInput;
    QSpinBox& maxResultsInput;

    QPushButton& searchButton;
};