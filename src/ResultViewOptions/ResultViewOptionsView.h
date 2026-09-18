#pragma once

#include "../MVP/View.h"

#include <QWidget>

class QComboBox;
class QLineEdit;
class QCheckBox;

class ResultViewOptionsView : public QWidget, public View
{
    Q_OBJECT

public:
    ResultViewOptionsView(QWidget* parent = nullptr);
    ~ResultViewOptionsView() = default;

public:
    // View
    QWidget& widget() override;
    const QWidget& widget() const override;

public:
    QComboBox& sortMode;

    QComboBox& filterMode;
    QLineEdit& filterLineEdit;

    QCheckBox& unreviewedCheckBox;
    QCheckBox& ignoredCheckBox;
    QCheckBox& interestedCheckBox;
};