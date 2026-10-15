#include "ResultViewOptionsView.h"

#include <QBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>

#include <CallableComboBox.hpp>


class Vacancy;

ResultViewOptionsView::ResultViewOptionsView(QWidget* parent) :
    QWidget(parent),
    filterMode(*new CallableComboBox<VacancyFilter>()),
    sortMode(*new CallableComboBox<VacancySortComparator>()),
    filterLineEdit(*new QLineEdit),
    unreviewedCheckBox(*new QCheckBox),
    ignoredCheckBox(*new QCheckBox),
    interestedCheckBox(*new QCheckBox)
{
    QHBoxLayout& mainLayout = *new QHBoxLayout;


    // Filter Layout
    QVBoxLayout& filterLayout = *new QVBoxLayout;

    QLabel& filterLabel = *new QLabel(tr("Filter (separate with \",\"):"));

    filterLayout.addWidget(&filterMode);
    filterLayout.addWidget(&filterLabel);
    filterLayout.addWidget(&filterLineEdit);

    mainLayout.addLayout(&filterLayout);


    // Sort Layout
    QVBoxLayout& sortLayout = *new QVBoxLayout;

    QLabel& sortLabel = *new QLabel(tr("Sort by:"));
    sortLayout.addWidget(&sortLabel);
    sortLayout.addWidget(&sortMode);

    mainLayout.addLayout(&sortLayout);


    // Visible Vacancies Layout
    QVBoxLayout& visibleVacanciesLayout = *new QVBoxLayout;
    QHBoxLayout& visibleVacanciesOptions = *new QHBoxLayout;
    
    QLabel& visibleVacanciesLabel = *new QLabel(tr("Show:"));

    unreviewedCheckBox.setText(tr("Unreviewed"));
    ignoredCheckBox.setText(tr("Ignored"));
    interestedCheckBox.setText(tr("Interested"));

    visibleVacanciesOptions.addWidget(&unreviewedCheckBox);
    visibleVacanciesOptions.addWidget(&ignoredCheckBox);
    visibleVacanciesOptions.addWidget(&interestedCheckBox);

    visibleVacanciesLayout.addWidget(&visibleVacanciesLabel);
    visibleVacanciesLayout.addLayout(&visibleVacanciesOptions);

    mainLayout.addLayout(&visibleVacanciesLayout);
    

    setLayout(&mainLayout);
}

QWidget& ResultViewOptionsView::widget()
{
    return *this;
}

const QWidget& ResultViewOptionsView::widget() const
{
    return *this;
}
