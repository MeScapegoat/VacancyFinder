// std
#include <iostream>

// Qt
#include <QCoreApplication> // TODO swap to gui
#include <QDir>
#include <QUrl>
#include <QUrlQuery>
#include <QFile>
#include <QTextStream>

// custom
#include "App.h"
#include "Logger.h"

int main(int argc, char *argv[])
{
    QCoreApplication coreApplication(argc, argv);

    QString logRulesPath("log_rules.ini");
    Linked::Logger logger;
    logger.setLogRulesPath(logRulesPath);

    Linked::App app;
    QTextStream in(stdin);
    QTextStream out(stdout);
    QString keyword, location;
    out << "Enter keyword: " << Qt::endl;;
    in >> keyword;
    out << "Enter location: " << Qt::endl;
    in >> location;
    app.sendSearchRequest(keyword, location);
    return coreApplication.exec();
}