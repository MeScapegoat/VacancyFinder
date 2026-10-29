
#ifdef _WIN32
#include <windows.h>
#endif

#include <iostream>

#include <QApplication>
#include <QDir>
#include <QUrl>
#include <QUrlQuery>
#include <QFile>
#include <QThread>

#include <VacancyFinder.h>
#include <Logger.h>

int main(int argc, char *argv[])
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    QThread::currentThread()->setObjectName("AppThread");
    QApplication application(argc, argv);
    //application.setStyle("windows"); // ???

    QString logRulesPath("log_rules.ini");
    Logger logger;
    logger.setLogRulesPath(logRulesPath);

    VacancyFinder app;

    return application.exec();
}
