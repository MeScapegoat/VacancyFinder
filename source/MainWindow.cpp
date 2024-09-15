// std
// Qt
#include <QCloseEvent>
#include <QApplication>
#include <QThread>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QStringListModel>

// custom
#include "MainWindow.h"

namespace Linked
{
    Q_LOGGING_CATEGORY(LinkedGUIMainWindow, "gui.main_window");

    void MainWindow::appendList(const Vacancy &vacancy)
    {
        if (!vacanciesList)
        {
            qCCritical(LinkedGUIMainWindow) << "List of vacancies not created";
            return;
        }
        auto model = qobject_cast<QStringListModel *>(vacanciesList->model());
        auto data = model->stringList();
        data.append(QString("%1. %2\n%3").arg(data.size() + 1, 3).arg(vacancy.title).arg(vacancy.companyName, vacancy.companyName.size() + 8));
        model->setStringList(data);
    }

    void MainWindow::clearList()
    {
        if (!vacanciesList)
        {
            qCCritical(LinkedGUIMainWindow) << "List of vacancies not created";
            return;
        }
        auto model = qobject_cast<QStringListModel *>(vacanciesList->model());
        model->setStringList(QStringList());
    }

    MainWindow::MainWindow()
    {
        // Maybe all incredible code below must be refactored into many initSMTH(QWidget* parent) functions for the sake of redability

        auto screenSize = QApplication::screens()[0]->size() / 2;
        qsizetype x, y, w, h;
        w = screenSize.width();
        h = screenSize.height();
        x = w / 2;
        y = h / 2;
        setGeometry(x, y, w, h);

        QWidget *centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);

        QVBoxLayout *mainVLayout = new QVBoxLayout(centralWidget);

        /*
         * searchLayout
         */

        QHBoxLayout *searchLayout = new QHBoxLayout;
        mainVLayout->addLayout(searchLayout);

        QLabel *keywordsLabel = new QLabel("Keywords:");
        keywordsLineEdit = new QLineEdit("C++");
        QLabel *locationLabel = new QLabel("Location:");
        locationLineEdit = new QLineEdit("Serbia");
        searchButton = new QPushButton("Search");

        searchLayout->addWidget(keywordsLabel);
        searchLayout->addWidget(keywordsLineEdit);
        searchLayout->addWidget(locationLabel);
        searchLayout->addWidget(locationLineEdit);
        searchLayout->addWidget(searchButton);

        /*
         * searchLayout
         */

        /*
         * searchResultLayout
         */
        QHBoxLayout *searchResultLayout = new QHBoxLayout;
        mainVLayout->addLayout(searchResultLayout);

        /*
         * vacanciesListLayout
         */

        QVBoxLayout *vacanciesListLayout = new QVBoxLayout;
        searchResultLayout->addLayout(vacanciesListLayout, 20);

        /*
         * filterLayout
         */
        QLabel *filterLabel = new QLabel("filter (separate with \",\"):");
        vacanciesListLayout->addWidget(filterLabel);

        QHBoxLayout *filterLayout = new QHBoxLayout;
        filterLayout->setAlignment(Qt::AlignLeft);
        vacanciesListLayout->addLayout(filterLayout);

        QRadioButton *noFilterOption = new QRadioButton("off");
        QRadioButton *includeFilterOption = new QRadioButton("MUST include");
        QRadioButton *excludeFilterOption = new QRadioButton("MUST NOT include");

        filterLayout->addWidget(noFilterOption);
        filterLayout->addWidget(includeFilterOption);
        filterLayout->addWidget(excludeFilterOption);

        filterLineEdit_ = new QLineEdit;
        vacanciesListLayout->addWidget(filterLineEdit_);

        filterMode_ = new QButtonGroup;
        filterMode_->addButton(noFilterOption, 0);
        filterMode_->addButton(includeFilterOption, 1);
        filterMode_->addButton(excludeFilterOption, 2);

        noFilterOption->setChecked(true);
        /*
         * filterLayout
         */

        /*
         * sortLayout
         */
        QHBoxLayout *sortLayout = new QHBoxLayout;
        sortLayout->setAlignment(Qt::AlignLeft);
        vacanciesListLayout->addLayout(sortLayout);

        QLabel *sortLabel = new QLabel("Sort by:");
        QRadioButton *relevanceOption = new QRadioButton("Relevance");
        QRadioButton *timeOption = new QRadioButton("Time");
        QRadioButton *titleOption = new QRadioButton("Title");
        QRadioButton *companyOption = new QRadioButton("Company");
        QRadioButton *levelOption = new QRadioButton("Level");

        sortLayout->addWidget(sortLabel);
        sortLayout->addWidget(relevanceOption);
        sortLayout->addWidget(timeOption);
        sortLayout->addWidget(titleOption);
        sortLayout->addWidget(companyOption);
        sortLayout->addWidget(levelOption);

        sortMode_ = new QButtonGroup;
        sortMode_->addButton(relevanceOption, 1);
        sortMode_->addButton(timeOption, 2);
        sortMode_->addButton(titleOption, 3);
        sortMode_->addButton(companyOption, 4);
        sortMode_->addButton(levelOption, 5);

        relevanceOption->setChecked(true);
        /*
         * sortLayout
         */

        vacanciesList = new QListView;
        QStringListModel *model = new QStringListModel;
        vacanciesList->setModel(model);
        vacanciesListLayout->addWidget(vacanciesList);

        /*
         * vacanciesDescriptionLayout
         */
        QVBoxLayout *vacanciesDescriptionLayout = new QVBoxLayout;
        searchResultLayout->addLayout(vacanciesDescriptionLayout, 80);

        titleLabel_ = new QLabel("Vacancy Title");
        titleLabel_->setStyleSheet("QLabel { font-size: 22px; }");
        titleLabel_->setOpenExternalLinks(true);
        vacanciesDescriptionLayout->addWidget(titleLabel_);

        companyNameLabel_ = new QLabel("Vacancy Company");
        companyNameLabel_->setStyleSheet("QLabel { font-size: 22px; }");
        companyNameLabel_->setOpenExternalLinks(true);
        vacanciesDescriptionLayout->addWidget(companyNameLabel_);

        levelLabel_ = new QLabel("Vacancy level");
        levelLabel_->setStyleSheet("QLabel { font-size: 18px; }");
        levelLabel_->setOpenExternalLinks(true);
        vacanciesDescriptionLayout->addWidget(levelLabel_);

        vacancyDesc = new QTextBrowser;
        vacancyDesc->setStyleSheet("QTextBrowser { font-size: 16px; }");
        vacancyDesc->setHtml(R"(
        <!DOCTYPE html>
        <html lang="en">
        <body>
            <header>
                <h1>Choose vacancy from left list to see its description here</h1>
            </header>
        </body>
        </html>)");

        vacanciesDescriptionLayout->addWidget(vacancyDesc);

        /*
         * vacanciesDescriptionLayout
         */

        /*
         * searchResultLayout
         */

        show();
    }

    MainWindow::~MainWindow() {}

    void MainWindow::closeEvent(QCloseEvent *event)
    {
        qCInfo(LinkedGUIMainWindow) << "closeEvent";
        event->accept();
    }

} // namespace Linked