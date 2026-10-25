#include "MainWindow.h"

#include <QVBoxLayout>

MainWindow::MainWindow(QWidget& queryParametersWidget,
                       QWidget& resultViewOptionsWidget,
                       QWidget& vacancyBrowserWidget,
                       QWidget* parent) :
    QMainWindow(parent)
{
    const QSize maxSize(1920, 1080);

    setMinimumSize(maxSize * 0.75);
    setMaximumSize(maxSize);

    QWidget& centralWidget = *new QWidget(this);
    setCentralWidget(&centralWidget);

    QVBoxLayout& mainVLayout = *new QVBoxLayout(&centralWidget);
    mainVLayout.addWidget(&queryParametersWidget);
    mainVLayout.addWidget(&resultViewOptionsWidget);
    mainVLayout.addWidget(&vacancyBrowserWidget);

    show();
}
