// std
// Qt
#include <QApplication>
#include <QButtonGroup>
#include <QCloseEvent>
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

// custom
#include "../core/SharedContext.h"
#include "../core/App.h"
#include "../model/Vacancy.h"
#include "MainWindow.h"



Q_LOGGING_CATEGORY(MainWindowLog, "gui.main_window");

MainWindow::MainWindow(SharedContext* sharedContext) :
    context(sharedContext),
    limitVacanciesInput(*new QSpinBox),
    keywordsLineEdit(*new QLineEdit),
    locationLineEdit(*new QLineEdit),
    searchButton(*new QPushButton),
    sortMode(*new QButtonGroup),
    filterMode(*new QButtonGroup),
    filterLineEdit(*new QLineEdit),
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
    expirationDateLabel(*new QLabel)
{
    qCDebug(MainWindowLog) << __FUNCTION__;
    // Maybe all incredible code below must be refactored into many initSMTH(QWidget* parent) functions for the sake of redability

    setMinimumSize(1024, 768);

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
    keywordsLineEdit.setText("C++");
    locationLineEdit.setText("Serbia");
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
    searchResultLayout.addLayout(&vacanciesListLayout, 20);

    /*
     * limitLayout
     */
    QVBoxLayout& limitLayout = *new QVBoxLayout;
    vacanciesListLayout.addLayout(&limitLayout);

    QLabel& limitLabel = *new QLabel("Max amount of processed vacancies:");

    limitVacanciesInput.setRange(1, 999);
    limitVacanciesInput.setValue(999);

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

    QRadioButton& noFilterOption = *new QRadioButton("off");
    QRadioButton& includeFilterOption = *new QRadioButton("MUST include");
    QRadioButton& excludeFilterOption = *new QRadioButton("MUST NOT include");

    filterLayout.addWidget(&noFilterOption);
    filterLayout.addWidget(&includeFilterOption);
    filterLayout.addWidget(&excludeFilterOption);

    vacanciesListLayout.addWidget(&filterLineEdit);

    filterMode.addButton(&noFilterOption, 0);
    filterMode.addButton(&includeFilterOption, 1);
    filterMode.addButton(&excludeFilterOption, 2);

    noFilterOption.setChecked(true);
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
    QRadioButton& relevanceOption = *new QRadioButton("Relevance");
    QRadioButton& timeOption = *new QRadioButton("Time");
    QRadioButton& titleOption = *new QRadioButton("Title");
    QRadioButton& companyOption = *new QRadioButton("Company");
    QRadioButton& levelOption = *new QRadioButton("Level");

    sortLayout.addWidget(&sortLabel);
    sortLayout.addWidget(&relevanceOption);
    sortLayout.addWidget(&timeOption);
    sortLayout.addWidget(&titleOption);
    sortLayout.addWidget(&companyOption);
    sortLayout.addWidget(&levelOption);

    sortMode.addButton(&relevanceOption, 1);
    sortMode.addButton(&timeOption, 2);
    sortMode.addButton(&titleOption, 3);
    sortMode.addButton(&companyOption, 4);
    sortMode.addButton(&levelOption, 5);

    relevanceOption.setChecked(true);
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
    searchResultLayout.addLayout(&vacanciesDescriptionLayout, 80);

    titleLabel.setText("Title");
    titleLabel.setStyleSheet("QLabel { font-size: 22px; }");
    titleLabel.setOpenExternalLinks(true);
    vacanciesDescriptionLayout.addWidget(&titleLabel);

    companyNameLabel.setText("Company");
    companyNameLabel.setStyleSheet("QLabel { font-size: 22px; }");
    companyNameLabel.setOpenExternalLinks(true);
    vacanciesDescriptionLayout.addWidget(&companyNameLabel);

    employmentTypeLabel.setText("Employment Type");
    employmentTypeLabel.setStyleSheet("QLabel { font-size: 18px; }");
    vacanciesDescriptionLayout.addWidget(&employmentTypeLabel);

    levelLabel.setText("Level");
    levelLabel.setStyleSheet("QLabel { font-size: 18px; }");
    vacanciesDescriptionLayout.addWidget(&levelLabel);

    experienceLabel.setText("Experience");
    experienceLabel.setStyleSheet("QLabel { font-size: 18px; }");
    vacanciesDescriptionLayout.addWidget(&experienceLabel);

    educationLabel.setText("Education");
    educationLabel.setStyleSheet("QLabel { font-size: 18px; }");
    vacanciesDescriptionLayout.addWidget(&educationLabel);

    skillsLabel.setText("Skills");
    skillsLabel.setStyleSheet("QLabel { font-size: 18px; }");
    vacanciesDescriptionLayout.addWidget(&skillsLabel);

    industryLabel.setText("Industry");
    industryLabel.setStyleSheet("QLabel { font-size: 18px; }");
    vacanciesDescriptionLayout.addWidget(&industryLabel);    

    expirationDateLabel.setText("Expiration Date");
    expirationDateLabel.setStyleSheet("QLabel { font-size: 18px; }");
    vacanciesDescriptionLayout.addWidget(&expirationDateLabel);


    vacancyDesc.setStyleSheet("QTextBrowser { font-size: 16px; }");
    vacancyDesc.setHtml(R"(
        <!DOCTYPE html>
        <html lang="en">
        <body>
            <header>
                <h1>Choose vacancy from left list to see its description here</h1>
            </header>
        </body>
        </html>)");

    vacanciesDescriptionLayout.addWidget(&vacancyDesc);

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

void MainWindow::updateVacanciesList()
{
    QStringListModel* model = qobject_cast<QStringListModel*>(vacanciesList.model());

    const QList<const Vacancy*> vacancies = context->app->getVacanciestoShow();

    QStringList data;
    for (const Vacancy* vacancy : vacancies)
    {
        const QString& id = QString::number(data.size() + 1);
        const QString& title = vacancy->brief.title;
        const QString& companyName = vacancy->brief.companyName;
        const qsizetype& companyNameWidth = companyName.size() + 8;
        const QString& age = QString::number(vacancy->brief.date.daysTo(QDate::currentDate()));
        const QString& location = vacancy->brief.location;

        data.append(QString("%1. %2\n%3\n%4\n%5 days ago")
                    .arg(id.leftJustified(3))
                    .arg(title)
                    .arg(companyName)
                    .arg(location)
                    .arg(age));
    }
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