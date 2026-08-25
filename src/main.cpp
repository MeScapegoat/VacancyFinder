
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
#include "core/App.h"
#include "core/Logger.h"

int main(int argc, char *argv[])
{
    SetConsoleOutputCP(CP_UTF8);

    QThread::currentThread()->setObjectName("AppThread");
    QApplication application(argc, argv);
    //application.setStyle("windows"); // ???

    QString logRulesPath("log_rules.ini");
    Linked::Logger logger;
    logger.setLogRulesPath(logRulesPath);

    Linked::App app;

    return application.exec();
}