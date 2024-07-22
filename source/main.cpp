// std

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
    QCoreApplication coreApplication(argc, argv); // TODO swap to gui
    qWarning() << "App started!";

    QString logRulesPath("log_rules.ini");
    Linked::Logger logger;
    logger.setLogRulesPath(logRulesPath);

    Linked::App app;
    app.sendSearchRequest("C++", "Serbia");

    // {
    //     QList<LinkedVacancy> vacancies;

    //     QDir titleFolder("../titles", "*.html");
    //     auto titles = titleFolder.entryList(QDir::Files, QDir::Name);
    //     QString titleFileName = "%1_title_response.html";

    //     qInfo() << "titles start";
    //     for (auto id = 0; id < titles.size(); ++id)
    //     {
    //         QString titleContent = Linked::App.loadTitleFromFile(titleFolder.absoluteFilePath(titleFileName.arg(id * 10, 5, 10, QChar('0'))));
    //         auto data = titleContent.split("<li>");
    //         for (auto n = 1; n < data.size(); ++n)
    //         {
    //             LinkedVacancy vacancy(data[n]);
    //             vacancies << vacancy;
    //         }
    //     }
    //     qInfo() << "titles end";

    //     qInfo() << "desc start";
    //     QDir descriptionFolder("../descriptions", "*.html");
    //     QString descriptionFileName = "%1_description_response.html";

    //     for (auto id = 0; id < vacancies.size(); ++id)
    //     {
    //         QString descriptionContent = Linked::App.loadDescriptionFromFile(descriptionFolder.absoluteFilePath(descriptionFileName.arg(id, 5, 10, QChar('0'))));
    //         vacancies[id].loadDescription(descriptionContent);
    //     }
    //     qInfo() << "desc end";

    //     Linked::App.vacancies = vacancies;

    //     QFile out("timeSorted");
    //     if (out.open(QIODevice::WriteOnly))
    //     {
    //         Linked::App.sortByTime();
    //         QTextStream outStream(&out);
    //         for (auto vacancy : Linked::App.vacancies)
    //             vacancy.print(outStream);
    //         out.close();
    //     }
    //     else
    //         qInfo() << "couldn't open" << out.fileName();

    //     out.setFileName("relevanceSorted");
    //     if (out.open(QIODevice::WriteOnly))
    //     {
    //         Linked::App.sortByRelevance("C++");
    //         QTextStream outStream(&out);
    //         for (auto vacancy : Linked::App.vacancies)
    //             vacancy.print(outStream);
    //         out.close();
    //     }
    //     else
    //         qInfo() << "couldn't open" << out.fileName();

    //     out.setFileName("titleSorted");
    //     if (out.open(QIODevice::WriteOnly))
    //     {
    //         Linked::App.sortByTitle();
    //         QTextStream outStream(&out);
    //         for (auto vacancy : Linked::App.vacancies)
    //             vacancy.print(outStream);
    //         out.close();
    //     }
    //     else
    //         qInfo() << "couldn't open" << out.fileName();

    //     out.setFileName("companySorted");
    //     if (out.open(QIODevice::WriteOnly))
    //     {
    //         Linked::App.sortByCompany();
    //         QTextStream outStream(&out);
    //         for (auto vacancy : Linked::App.vacancies)
    //             vacancy.print(outStream);
    //         out.close();
    //     }
    //     else
    //         qInfo() << "couldn't open" << out.fileName();
    // }

    qWarning() << "App finished!";
    return coreApplication.exec();
}