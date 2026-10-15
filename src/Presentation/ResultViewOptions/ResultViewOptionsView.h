#pragma once

#include <View.h>

#include <QWidget>

#include <FunctionTypes.h>

template<typename T>
class CallableComboBox;
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
    CallableComboBox<VacancyFilter>& filterMode;
    QLineEdit& filterLineEdit;

    CallableComboBox<VacancySortComparator>& sortMode;

    QCheckBox& unreviewedCheckBox;
    QCheckBox& ignoredCheckBox;
    QCheckBox& interestedCheckBox;
};