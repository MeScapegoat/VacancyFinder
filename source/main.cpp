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
#include "App.h"
#include "Logger.h"

int main(int argc, char *argv[])
{
    QThread::currentThread()->setObjectName("MainThread");
    QApplication application(argc, argv);
    //application.setStyle("windows"); // ???

    QString logRulesPath("log_rules.ini");
    Linked::Logger logger;
    logger.setLogRulesPath(logRulesPath);

    Linked::App app;

    return application.exec();
}