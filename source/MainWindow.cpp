// std
// Qt
#include <QCloseEvent>
#include <QApplication>
#include <QThread>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QLabel>
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
        data.append(QString("%1. %2").arg(data.size() + 1).arg(vacancy.title));
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
         * sortLayout
         */
        QHBoxLayout *sortLayout = new QHBoxLayout;
        sortLayout->setAlignment(Qt::AlignLeft);
        mainVLayout->addLayout(sortLayout);

        QLabel *sortLabel = new QLabel("Sort by:");
        QRadioButton *relevanceOption = new QRadioButton("Relevance");
        QRadioButton *timeOption = new QRadioButton("Time");
        QRadioButton *titleOption = new QRadioButton("Title");
        QRadioButton *companyOption = new QRadioButton("Company");

        sortLayout->addWidget(sortLabel);
        sortLayout->addWidget(relevanceOption);
        sortLayout->addWidget(timeOption);
        sortLayout->addWidget(titleOption);
        sortLayout->addWidget(companyOption);

        sortMode_ = new QButtonGroup;
        sortMode_->addButton(relevanceOption, 1);
        sortMode_->addButton(timeOption, 2);
        sortMode_->addButton(titleOption, 3);
        sortMode_->addButton(companyOption, 4);

        relevanceOption->setChecked(true);
        /*
         * sortLayout
         */

        /*
         * searchResultLayout
         */
        QHBoxLayout *searchResultLayout = new QHBoxLayout;
        mainVLayout->addLayout(searchResultLayout);

        vacanciesList = new QListView;
        QStringListModel *model = new QStringListModel;
        vacanciesList->setModel(model);
        vacancyDesc = new QTextBrowser;
        vacancyDesc->setHtml(R"(
        <!DOCTYPE html>
        <html lang="en">
        <body>
            <header>
                <h1>Choose vacancy from left list to see its description here</h1>
            </header>
        </body>
        </html>)");

        searchResultLayout->addWidget(vacanciesList, 20);
        searchResultLayout->addWidget(vacancyDesc, 80);

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