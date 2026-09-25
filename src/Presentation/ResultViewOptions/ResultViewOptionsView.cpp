#include "ResultViewOptionsView.h"

#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>

#include "ResultViewOptionsEnums.h"

ResultViewOptionsView::ResultViewOptionsView(QWidget* parent) :
    QWidget(parent),
    sortMode(*new QComboBox),
    filterMode(*new QComboBox),
    filterLineEdit(*new QLineEdit),
    unreviewedCheckBox(*new QCheckBox),
    ignoredCheckBox(*new QCheckBox),
    interestedCheckBox(*new QCheckBox)
{
    QHBoxLayout& mainLayout = *new QHBoxLayout;


    // Filter Layout
    QVBoxLayout& filterLayout = *new QVBoxLayout;

    filterMode.insertItem(static_cast<qsizetype>(FilterMode::Off), QObject::tr("Off"));
    filterMode.insertItem(static_cast<qsizetype>(FilterMode::Include), QObject::tr("Include"));
    filterMode.insertItem(static_cast<qsizetype>(FilterMode::Exclude), QObject::tr("Exclude"));

    QLabel& filterLabel = *new QLabel(tr("Filter (separate with \",\"):"));

    filterLayout.addWidget(&filterMode);
    filterLayout.addWidget(&filterLabel);
    filterLayout.addWidget(&filterLineEdit);

    mainLayout.addLayout(&filterLayout);


    // Sort Layout
    QVBoxLayout& sortLayout = *new QVBoxLayout;

    QLabel& sortLabel = *new QLabel(tr("Sort by:"));
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Off), tr("Off"));
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Relevance), tr("Relevance"));
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Title), tr("Title"));
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Company), tr("Company"));
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Time), tr("Time"));
    sortMode.insertItem(static_cast<qsizetype>(SortMode::EmploymentType), tr("EmploymentType"));
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Level), tr("Level"));
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Experience), tr("Experience"));
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Education), tr("Education"));
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Skills), tr("Skills"));
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Industry), tr("Industry"));
    sortMode.insertItem(static_cast<qsizetype>(SortMode::ExpirationDate), tr("ExpirationDate"));

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
