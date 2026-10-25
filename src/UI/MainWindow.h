#pragma once
#include <QMainWindow>


class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget& queryParametersWidget,
               QWidget& resultViewOptionsWidget,
               QWidget& vacancyBrowserWidget,
               QWidget* parent = nullptr);
};
