
#include <windows.h>

// std
#include <iostream>

// Qt
#include <QApplication>
#include <QDir>
#include <QUrl>
#include <QUrlQuery>
#include <QFile>
#include <QThread>

// custom
#include <App.h>
#include <Logger.h>

int main(int argc, char *argv[])
{
    SetConsoleOutputCP(CP_UTF8);

    QThread::currentThread()->setObjectName("AppThread");
    QApplication application(argc, argv);
    //application.setStyle("windows"); // ???

    QString logRulesPath("log_rules.ini");
    Logger logger;
    logger.setLogRulesPath(logRulesPath);

    App app;

    return application.exec();
}