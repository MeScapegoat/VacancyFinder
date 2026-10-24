#pragma once

#include <View.h>

#include <QWidget>

#include <FunctionTypes.h>

template<typename T>
class CallableComboBox;
class QLineEdit;
class QCheckBox;
class QString;

class ResultViewOptionsView : public View
{
    Q_OBJECT

public:
    ResultViewOptionsView(QWidget* parent = nullptr);
    ~ResultViewOptionsView() = default;

public:
    const CallableComboBox<VacancyFilter>& filterMode() const;
    CallableComboBox<VacancyFilter>& filterMode();

    const QLineEdit& filterLineEdit() const;
    QLineEdit& filterLineEdit();

    const CallableComboBox<VacancySortComparator>& sortMode() const;
    CallableComboBox<VacancySortComparator>& sortMode();

    const QCheckBox& unreviewedCheckBox() const;
    QCheckBox& unreviewedCheckBox();

    const QCheckBox& ignoredCheckBox() const;
    QCheckBox& ignoredCheckBox();

    const QCheckBox& interestedCheckBox() const;
    QCheckBox& interestedCheckBox();

    void setFilterMode(int index);
    void setFilterLineEdit(const QString& filterWords);
    void setSortMode(int index);
    void setUnreviewedCheckBox(bool isChecked);
    void setIgnoredCheckBox(bool isChecked);
    void setInterestedCheckBox(bool isChecked);

private:
    CallableComboBox<VacancyFilter>& m_filterMode;
    QLineEdit& m_filterLineEdit;

    CallableComboBox<VacancySortComparator>& m_sortMode;

    QCheckBox& m_unreviewedCheckBox;
    QCheckBox& m_ignoredCheckBox;
    QCheckBox& m_interestedCheckBox;
};
