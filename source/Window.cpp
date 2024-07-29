// std
// Qt
#include <QCloseEvent>
#include <QApplication>
#include <QThread>

#include <QHBoxLayout>

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QListView>

// custom
#include "Window.h"

namespace Linked
{
    Q_LOGGING_CATEGORY(LinkedGUIWindow, "gui.window");

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

        QLabel *keywordsLabel = new QLabel("KeywordsLabel", centralWidget);
        QLineEdit *keywordsLineEdit = new QLineEdit("KeywordsLineEdit", centralWidget);
        QLabel *locationLabel = new QLabel("LocationLabel", centralWidget);
        QLineEdit *locationLineEdit = new QLineEdit("LocationLineEdit", centralWidget);
        QPushButton *searchButton = new QPushButton("SearchButton", centralWidget);

        QHBoxLayout *searchLayout = new QHBoxLayout(centralWidget);
        searchLayout->addWidget(keywordsLabel);
        searchLayout->addWidget(keywordsLineEdit);
        searchLayout->addWidget(locationLabel);
        searchLayout->addWidget(locationLineEdit);
        searchLayout->addWidget(searchButton);


        QListView * vacanciesList = new QListView(centralWidget);

        QHBoxLayout *searchResultLayout = new QHBoxLayout(centralWidget);
        searchResultLayout->addWidget(vacanciesList);


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