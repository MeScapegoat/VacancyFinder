// std
// Qt
#include <QApplication>
#include <QButtonGroup>
#include <QCheckBox>
#include <QCloseEvent>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListView>
#include <QPushButton>
#include <QRadioButton>
#include <QSpinBox>
#include <QStringListModel>
#include <QTextBrowser>
#include <QThread>
#include <QVBoxLayout>
#include <QStackedLayout>

// custom
#include "../core/App.h"
#include "../model/Vacancy.h"
#include "MainWindow.h"



Q_LOGGING_CATEGORY(MainWindowLog, "gui.main_window");

MainWindow::MainWindow() :
    limitVacanciesInput(*new QSpinBox),
    keywordsLineEdit(*new QLineEdit),
    locationLineEdit(*new QLineEdit),
    searchButton(*new QPushButton),
    sortMode(*new QComboBox),
    filterMode(*new QComboBox),
    filterLineEdit(*new QLineEdit),
    unreviewedCheckBox(*new QCheckBox),
    ignoredCheckBox(*new QCheckBox),
    interestedCheckBox(*new QCheckBox),
    vacanciesList(*new QListView),
    titleLabel(*new QLabel),
    companyNameLabel(*new QLabel),
    employmentTypeLabel(*new QLabel),
    levelLabel(*new QLabel),
    experienceLabel(*new QLabel),
    vacancyDesc(*new QTextBrowser),
    educationLabel(*new QLabel),
    skillsLabel(*new QLabel),
    industryLabel(*new QLabel),
    expirationDateLabel(*new QLabel),
    statusButtonsLayout(*new QStackedLayout),
    ignoreButton(*new QPushButton),
    interestButton(*new QPushButton),
    removeIgnoreButton(*new QPushButton),
    removeInterestButton(*new QPushButton)
{
    qCDebug(MainWindowLog) << __FUNCTION__;
    // TODO: Maybe all incredible code below must be refactored into many initSMTH(QWidget* parent) functions for the sake of redability
    // TODO: Well there is a lot to change in UI and code...

    const QSize maxSize(1920, 1080);

    setMinimumSize(maxSize * 0.75);
    setMaximumSize(maxSize);

    QWidget& centralWidget = *new QWidget(this);
    setCentralWidget(&centralWidget);

    QVBoxLayout& mainVLayout = *new QVBoxLayout(&centralWidget);

    /*
     * searchLayout
     */

    QHBoxLayout& searchLayout = *new QHBoxLayout;
    mainVLayout.addLayout(&searchLayout);

    QLabel& keywordsLabel = *new QLabel("Keywords:");
    QLabel& locationLabel = *new QLabel("Location:");
    searchButton.setText("Search");

    searchLayout.addWidget(&keywordsLabel);
    searchLayout.addWidget(&keywordsLineEdit);
    searchLayout.addWidget(&locationLabel);
    searchLayout.addWidget(&locationLineEdit);
    searchLayout.addWidget(&searchButton);

    /*
     * searchLayout
     */

     /*
      * searchResultLayout
      */
    QHBoxLayout& searchResultLayout = *new QHBoxLayout;
    mainVLayout.addLayout(&searchResultLayout);

    /*
     * vacanciesListLayout
     */

    QVBoxLayout& vacanciesListLayout = *new QVBoxLayout;
    searchResultLayout.addLayout(&vacanciesListLayout);

    /*
     * limitLayout
     */
    QVBoxLayout& limitLayout = *new QVBoxLayout;
    vacanciesListLayout.addLayout(&limitLayout);

    QLabel& limitLabel = *new QLabel("Max amount of processed vacancies:");

    limitVacanciesInput.setRange(1, 999);

    limitLayout.addWidget(&limitLabel);
    limitLayout.addWidget(&limitVacanciesInput);

    /*
     * limitLayout
     */

     /*
      * filterLayout
      */
    QLabel& filterLabel = *new QLabel("filter (separate with \",\"):");
    vacanciesListLayout.addWidget(&filterLabel);

    QHBoxLayout& filterLayout = *new QHBoxLayout;
    filterLayout.setAlignment(Qt::AlignLeft);
    vacanciesListLayout.addLayout(&filterLayout);


    filterMode.insertItem(static_cast<qsizetype>(FilterMode::Off), "Off");
    filterMode.insertItem(static_cast<qsizetype>(FilterMode::Include), "Include");
    filterMode.insertItem(static_cast<qsizetype>(FilterMode::Exclude), "Exclude");

    filterLayout.addWidget(&filterMode);

    vacanciesListLayout.addWidget(&filterLineEdit);
    /*
     * filterLayout
     */

     /*
      * sortLayout
      */
    QHBoxLayout& sortLayout = *new QHBoxLayout;
    sortLayout.setAlignment(Qt::AlignLeft);
    vacanciesListLayout.addLayout(&sortLayout);

    QLabel& sortLabel = *new QLabel("Sort by:");
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Off),            "Off");
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Relevance),      "Relevance");
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Title),          "Title");
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Company),        "Company");
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Time),           "Time");
    sortMode.insertItem(static_cast<qsizetype>(SortMode::EmploymentType), "EmploymentType");
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Level),          "Level");
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Experience),     "Experience");
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Education),      "Education");
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Skills),         "Skills");
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Industry),       "Industry");
    sortMode.insertItem(static_cast<qsizetype>(SortMode::ExpirationDate), "ExpirationDate");

    sortLayout.addWidget(&sortLabel);
    sortLayout.addWidget(&sortMode);


    QLabel& visibleVacanciesLabel = *new QLabel("Show:");
    unreviewedCheckBox.setText("Unreviewed");
    ignoredCheckBox.setText("Ignored");
    interestedCheckBox.setText("Interested");

    QHBoxLayout& visibleVacanciesLayout = *new QHBoxLayout;
    visibleVacanciesLayout.addWidget(&visibleVacanciesLabel);
    visibleVacanciesLayout.addWidget(&unreviewedCheckBox);
    visibleVacanciesLayout.addWidget(&ignoredCheckBox);
    visibleVacanciesLayout.addWidget(&interestedCheckBox);

    vacanciesListLayout.addLayout(&visibleVacanciesLayout);


    
    /*
     * sortLayout
     */

    QStringListModel& model = *new QStringListModel;
    vacanciesList.setModel(&model);
    vacanciesListLayout.addWidget(&vacanciesList);

    /*
     * vacanciesDescriptionLayout
     */
    QVBoxLayout& vacanciesDescriptionLayout = *new QVBoxLayout;
    searchResultLayout.addLayout(&vacanciesDescriptionLayout);

    titleLabel.setStyleSheet("QLabel { font-size: 22px; }");
    titleLabel.setOpenExternalLinks(true);
    vacanciesDescriptionLayout.addWidget(&titleLabel);

    companyNameLabel.setStyleSheet("QLabel { font-size: 22px; }");
    companyNameLabel.setOpenExternalLinks(true);
    vacanciesDescriptionLayout.addWidget(&companyNameLabel);

    employmentTypeLabel.setStyleSheet("QLabel { font-size: 18px; }");
    vacanciesDescriptionLayout.addWidget(&employmentTypeLabel);

    levelLabel.setStyleSheet("QLabel { font-size: 18px; }");
    vacanciesDescriptionLayout.addWidget(&levelLabel);

    experienceLabel.setStyleSheet("QLabel { font-size: 18px; }");
    vacanciesDescriptionLayout.addWidget(&experienceLabel);

    educationLabel.setStyleSheet("QLabel { font-size: 18px; }");
    vacanciesDescriptionLayout.addWidget(&educationLabel);

    skillsLabel.setStyleSheet("QLabel { font-size: 18px; }");
    vacanciesDescriptionLayout.addWidget(&skillsLabel);

    industryLabel.setStyleSheet("QLabel { font-size: 18px; }");
    vacanciesDescriptionLayout.addWidget(&industryLabel);    

    expirationDateLabel.setStyleSheet("QLabel { font-size: 18px; }");
    vacanciesDescriptionLayout.addWidget(&expirationDateLabel);


    vacancyDesc.setStyleSheet("QTextBrowser { font-size: 16px; }");
    vacanciesDescriptionLayout.addWidget(&vacancyDesc, 1);


    ignoreButton.setText("Ignore");
    interestButton.setText("I am interested");

    removeIgnoreButton.setText("Remove from ignore list");
    removeInterestButton.setText("Remove from interested list");

    QHBoxLayout& unreviewedLayout = *new QHBoxLayout;
    unreviewedLayout.addWidget(&ignoreButton);
    unreviewedLayout.addWidget(&interestButton);

    QWidget& unreviewedWidget = *new QWidget;
    unreviewedWidget.setLayout(&unreviewedLayout);

    statusButtonsLayout.insertWidget(static_cast<int>(VacancyStatus::Unreviewed), &unreviewedWidget);
    statusButtonsLayout.insertWidget(static_cast<int>(VacancyStatus::Ignored), &removeIgnoreButton);
    statusButtonsLayout.insertWidget(static_cast<int>(VacancyStatus::Interested), &removeInterestButton);

    vacanciesDescriptionLayout.addLayout(&statusButtonsLayout);

    /*
     * vacanciesDescriptionLayout
     */

     /*
      * searchResultLayout
      */

    show();
}

MainWindow::~MainWindow()
{
    qCDebug(MainWindowLog) << __FUNCTION__;
}

void MainWindow::updateVacanciesList(const QStringList& data)
{
    QStringListModel* model = qobject_cast<QStringListModel*>(vacanciesList.model());

    model->setStringList(data);
}

void MainWindow::clearList()
{
    auto model = qobject_cast<QStringListModel*>(vacanciesList.model());
    model->setStringList(QStringList());
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    qCInfo(MainWindowLog) << "closeEvent";
    event->accept();
}