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
#include <QStringListModel>

// custom
#include "MainWindow.h"

namespace Linked
{
    Q_LOGGING_CATEGORY(LinkedGUIMainWindow, "gui.main_window");

    void MainWindow::test()
    {
        qCInfo(LinkedGUIMainWindow) << "Test button pressed";

        if (!vacanciesList)
        {
            qCCritical(LinkedGUIMainWindow) << "List of vacancies not created";
            return;
        }
        auto model = qobject_cast<QStringListModel *>(vacanciesList->model());
        auto data = model->stringList();
        data.append("TEST");
        model->setStringList(data);
    }

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

        keywordsLineEdit = new QLineEdit("KeywordsLineEdit", centralWidget);
        locationLineEdit = new QLineEdit("LocationLineEdit", centralWidget);
        searchButton = new QPushButton("SearchButton", centralWidget);

        QLabel *locationLabel = new QLabel("LocationLabel", centralWidget);
        QLabel *keywordsLabel = new QLabel("KeywordsLabel", centralWidget);

        QPushButton *testButton = new QPushButton("TEST", centralWidget);
        QObject::connect(testButton, &QPushButton::pressed, this, &MainWindow::test);

        QHBoxLayout *searchLayout = new QHBoxLayout;
        mainVLayout->addLayout(searchLayout);
        searchLayout->addWidget(keywordsLabel);
        searchLayout->addWidget(keywordsLineEdit);
        searchLayout->addWidget(locationLabel);
        searchLayout->addWidget(locationLineEdit);
        searchLayout->addWidget(searchButton);
        searchLayout->addWidget(testButton);

        vacanciesList = new QListView(centralWidget);

        QStringListModel *model = new QStringListModel();
        vacanciesList->setModel(model);
        vacancyDesc = new QTextBrowser(centralWidget);
        vacancyDesc->setHtml(R"(
        <!DOCTYPE html>
        <html lang="en">
        <body>
            <header>
                <h1>Choose vacancy from left list to see its description here</h1>
            </header>
        </body>
        </html>)");

        QHBoxLayout *searchResultLayout = new QHBoxLayout;

        mainVLayout->addLayout(searchResultLayout);

        searchResultLayout->addWidget(vacanciesList, 20);
        searchResultLayout->addWidget(vacancyDesc, 80);

        show();
    }

    MainWindow::~MainWindow() {}

    void MainWindow::closeEvent(QCloseEvent *event)
    {
        qCInfo(LinkedGUIMainWindow) << "closeEvent";
        event->accept();
    }

} // namespace Linked