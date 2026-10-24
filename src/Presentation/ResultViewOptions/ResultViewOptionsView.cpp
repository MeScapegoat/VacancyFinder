#include "ResultViewOptionsView.h"

#include <QBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QString>

#include <CallableComboBox.hpp>


class Vacancy;

ResultViewOptionsView::ResultViewOptionsView(QWidget* parent) :
    View(parent),
    m_filterMode(*new CallableComboBox<VacancyFilter>()),
    m_sortMode(*new CallableComboBox<VacancySortComparator>()),
    m_filterLineEdit(*new QLineEdit),
    m_unreviewedCheckBox(*new QCheckBox),
    m_ignoredCheckBox(*new QCheckBox),
    m_interestedCheckBox(*new QCheckBox)
{
    QHBoxLayout& mainLayout = *new QHBoxLayout;


    // Filter Layout
    QVBoxLayout& filterLayout = *new QVBoxLayout;

    QLabel& filterLabel = *new QLabel(tr("Filter (separate with \",\"):"));

    filterLayout.addWidget(&m_filterMode);
    filterLayout.addWidget(&filterLabel);
    filterLayout.addWidget(&m_filterLineEdit);

    mainLayout.addLayout(&filterLayout);


    // Sort Layout
    QVBoxLayout& sortLayout = *new QVBoxLayout;

    QLabel& sortLabel = *new QLabel(tr("Sort by:"));
    sortLayout.addWidget(&sortLabel);
    sortLayout.addWidget(&m_sortMode);

    mainLayout.addLayout(&sortLayout);


    // Visible Vacancies Layout
    QVBoxLayout& visibleVacanciesLayout = *new QVBoxLayout;
    QHBoxLayout& visibleVacanciesOptions = *new QHBoxLayout;
    
    QLabel& visibleVacanciesLabel = *new QLabel(tr("Show:"));

    m_unreviewedCheckBox.setText(tr("Unreviewed"));
    m_ignoredCheckBox.setText(tr("Ignored"));
    m_interestedCheckBox.setText(tr("Interested"));

    visibleVacanciesOptions.addWidget(&m_unreviewedCheckBox);
    visibleVacanciesOptions.addWidget(&m_ignoredCheckBox);
    visibleVacanciesOptions.addWidget(&m_interestedCheckBox);

    visibleVacanciesLayout.addWidget(&visibleVacanciesLabel);
    visibleVacanciesLayout.addLayout(&visibleVacanciesOptions);

    mainLayout.addLayout(&visibleVacanciesLayout);
    

    setLayout(&mainLayout);
}

const CallableComboBox<VacancyFilter>& ResultViewOptionsView::filterMode() const
{
    return m_filterMode;
}

CallableComboBox<VacancyFilter>& ResultViewOptionsView::filterMode()
{
    return m_filterMode;
}

const QLineEdit& ResultViewOptionsView::filterLineEdit() const
{
    return m_filterLineEdit;
}

QLineEdit& ResultViewOptionsView::filterLineEdit()
{
    return m_filterLineEdit;
}

const CallableComboBox<VacancySortComparator>& ResultViewOptionsView::sortMode() const
{
    return m_sortMode;
}

CallableComboBox<VacancySortComparator>& ResultViewOptionsView::sortMode()
{
    return m_sortMode;
}

const QCheckBox& ResultViewOptionsView::unreviewedCheckBox() const
{
    return m_unreviewedCheckBox;
}

QCheckBox& ResultViewOptionsView::unreviewedCheckBox()
{
    return m_unreviewedCheckBox;
}

const QCheckBox& ResultViewOptionsView::ignoredCheckBox() const
{
    return m_ignoredCheckBox;
}

QCheckBox& ResultViewOptionsView::ignoredCheckBox()
{
    return m_ignoredCheckBox;
}

const QCheckBox& ResultViewOptionsView::interestedCheckBox() const
{
    return m_interestedCheckBox;
}

QCheckBox& ResultViewOptionsView::interestedCheckBox()
{
    return m_interestedCheckBox;
}

void ResultViewOptionsView::setFilterMode(int index)
{
    m_filterMode.setCurrentIndex(index);
}

void ResultViewOptionsView::setFilterLineEdit(const QString& filterWords)
{
    m_filterLineEdit.setText(filterWords);
}

void ResultViewOptionsView::setSortMode(int index)
{
    m_sortMode.setCurrentIndex(index);
}

void ResultViewOptionsView::setUnreviewedCheckBox(bool isChecked)
{
    m_unreviewedCheckBox.setChecked(isChecked);
}

void ResultViewOptionsView::setIgnoredCheckBox(bool isChecked)
{
    m_ignoredCheckBox.setChecked(isChecked);
}

void ResultViewOptionsView::setInterestedCheckBox(bool isChecked)
{
    m_interestedCheckBox.setChecked(isChecked);
}
