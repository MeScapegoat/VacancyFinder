#pragma once
#include <QLoggingCategory>
#include <QMainWindow>


Q_DECLARE_LOGGING_CATEGORY(MainWindowLog);


class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget& queryParametersWidget,
               QWidget& resultViewOptionsWidget,
               QWidget& vacancyBrowserWidget,
               QWidget* parent = nullptr);
};