// std
#include <iostream>
#include <fstream>

// Qt
#include <QCoreApplication> // TODO swap to gui
#include <QDir>
#include <QFile>
#include <QLoggingCategory>
#include <QTextStream>

// custom
#include <LinkedApp.h>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv); // TODO swap to gui
    std::cout << "App started!\n";

    LinkedApp linkedApp;
    linkedApp.sendSearchRequest();

    // {
    //     QList<LinkedVacancy> vacancies;

    //     QDir titleFolder("../titles", "*.html");
    //     auto titles = titleFolder.entryList(QDir::Files, QDir::Name);
    //     QString titleFileName = "%1_title_response.html";

    //     qInfo() << "titles start";
    //     for (auto id = 0; id < titles.size(); ++id)
    //     {
    //         QString titleContent = linkedApp.loadTitleFromFile(titleFolder.absoluteFilePath(titleFileName.arg(id * 10, 5, 10, QChar('0'))));
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
    //         QString descriptionContent = linkedApp.loadDescriptionFromFile(descriptionFolder.absoluteFilePath(descriptionFileName.arg(id, 5, 10, QChar('0'))));
    //         vacancies[id].loadDescription(descriptionContent);
    //     }
    //     qInfo() << "desc end";

    //     linkedApp.vacancies = vacancies;

    //     QFile out("timeSorted");
    //     if (out.open(QIODevice::WriteOnly))
    //     {
    //         linkedApp.sortByTime();
    //         QTextStream outStream(&out);
    //         for (auto vacancy : linkedApp.vacancies)
    //             vacancy.print(outStream);
    //         out.close();
    //     }
    //     else
    //         qInfo() << "couldn't open" << out.fileName();

    //     out.setFileName("relevanceSorted");
    //     if (out.open(QIODevice::WriteOnly))
    //     {
    //         linkedApp.sortByRelevance("C++");
    //         QTextStream outStream(&out);
    //         for (auto vacancy : linkedApp.vacancies)
    //             vacancy.print(outStream);
    //         out.close();
    //     }
    //     else
    //         qInfo() << "couldn't open" << out.fileName();

    //     out.setFileName("titleSorted");
    //     if (out.open(QIODevice::WriteOnly))
    //     {
    //         linkedApp.sortByTitle();
    //         QTextStream outStream(&out);
    //         for (auto vacancy : linkedApp.vacancies)
    //             vacancy.print(outStream);
    //         out.close();
    //     }
    //     else
    //         qInfo() << "couldn't open" << out.fileName();

    //     out.setFileName("companySorted");
    //     if (out.open(QIODevice::WriteOnly))
    //     {
    //         linkedApp.sortByCompany();
    //         QTextStream outStream(&out);
    //         for (auto vacancy : linkedApp.vacancies)
    //             vacancy.print(outStream);
    //         out.close();
    //     }
    //     else
    //         qInfo() << "couldn't open" << out.fileName();
    // }
    
    qInfo() << "App finished!\n";
    return app.exec();
}