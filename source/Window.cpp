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
#include "Window.h"

namespace Linked
{
    Q_LOGGING_CATEGORY(LinkedGUIWindow, "gui.window");

    void Window::test()
    {
        qCInfo(LinkedGUIWindow) << "Test button pressed";

        if (!vacanciesList)
        {
            qCCritical(LinkedGUIWindow) << "List of vacancies not created";
            return;
        }
        auto model = qobject_cast<QStringListModel *>(vacanciesList->model());
        auto data = model->stringList();
        data.append("TEST");
        model->setStringList(data);
    }

    Window::Window()
    {
        qCDebug(LinkedGUIWindow) << QThread::currentThread();

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
        QObject::connect(testButton, &QPushButton::pressed, this, &Window::test);

        QHBoxLayout *searchLayout = new QHBoxLayout;
        mainVLayout->addLayout(searchLayout);
        searchLayout->addWidget(keywordsLabel);
        searchLayout->addWidget(keywordsLineEdit);
        searchLayout->addWidget(locationLabel);
        searchLayout->addWidget(locationLineEdit);
        searchLayout->addWidget(searchButton);
        searchLayout->addWidget(testButton);

        vacanciesList = new QListView(centralWidget);

        QStringList data;
        data << "data 1" << "data 2" << "data 3" << "data 4" << "data 5";
        QStringListModel *model = new QStringListModel();
        model->setStringList(data);
        vacanciesList->setModel(model);
        vacancyDesc = new QWebEngineView(centralWidget);
        vacancyDesc->setHtml(R"(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Simple HTML Site</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            margin: 0;
            padding: 0;
        }
        header {
            background-color: #4CAF50;
            color: white;
            text-align: center;
            padding: 1em 0;
        }
        main {
            padding: 20px;
        }
        footer {
            background-color: #4CAF50;
            color: white;
            text-align: center;
            padding: 1em 0;
            position: fixed;
            width: 100%;
            bottom: 0;
        }
    </style>
</head>
<body>

<header>
    <h1>Welcome to My Simple Site</h1>
</header>

<main>
    <h2>About This Site</h2>
    <p>This is a simple HTML website created to demonstrate the basic structure of an HTML document. You can add more content and styles to make it your own.</p>

    <h2>Features</h2>
    <ul>
        <li>Simple layout</li>
        <li>Responsive design</li>
        <li>Easy to customize</li>
    </ul>
</main>

<footer>
    <p>&copy; 2024 My Simple Site</p>
</footer>

</body>
</html>
)");

        QHBoxLayout *searchResultLayout = new QHBoxLayout;

        mainVLayout->addLayout(searchResultLayout);

        searchResultLayout->addWidget(vacanciesList, 20);
        searchResultLayout->addWidget(vacancyDesc, 80);

        QObject::connect(vacanciesList, &QListView::clicked, [this](const QModelIndex &index)
                         { emit openVacancyDescription(index.row()); });
        QObject::connect(searchButton, &QPushButton::released, [this]()
                         { emit startSearch(keywordsLineEdit->text(), locationLineEdit->text()); });

        show();
    }

    Window::~Window()
    {
        qCDebug(LinkedGUIWindow) << QThread::currentThread();
    }

    void Window::closeEvent(QCloseEvent *event)
    {
        qCDebug(LinkedGUIWindow) << QThread::currentThread();
        qCInfo(LinkedGUIWindow) << "closeEvent";
        event->accept();
    }

} // namespace Linked