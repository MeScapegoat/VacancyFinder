// std
#include <iostream>
#include <fstream>

// Qt
#include <QCoreApplication> // TODO swap to gui
#include <QFile>

// custom
#include <LinkedApp.h>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv); // TODO swap to gui
    std::cout << "App started!\n";
    LinkedApp linkedApp(&app);
    linkedApp.sendSearchRequest();

    QFile relevanceSorted("relevanceSorted");
    if(relevanceSorted.open(QIODeviceBase::WriteOnly))
    {
        QTextStream relevanceSortedStream(&relevanceSorted);
        for (const auto& vacancy : linkedApp.getVacancies())
            vacancy.print(relevanceSortedStream);
        relevanceSorted.close();
    }

    linkedApp.sortByTime();

    QFile timeSorted("timeSorted");
    if(timeSorted.open(QIODeviceBase::WriteOnly))
    {
        QTextStream timeSortedStream(&timeSorted);
        for (const auto& vacancy : linkedApp.getVacancies())
            vacancy.print(timeSortedStream);
        timeSorted.close();
    }

    linkedApp.sortByTitle();

    QFile titleSorted("titleSorted");
    if(titleSorted.open(QIODeviceBase::WriteOnly))
    {
        QTextStream titleSortedStream(&titleSorted);
        for (const auto& vacancy : linkedApp.getVacancies())
            vacancy.print(titleSortedStream);
        titleSorted.close();
    }

    linkedApp.sortByCompany();

    QFile companySorted("companySorted");
    if(companySorted.open(QIODeviceBase::WriteOnly))
    {
        QTextStream companySortedStream(&companySorted);
        for (const auto& vacancy : linkedApp.getVacancies())
            vacancy.print(companySortedStream);
        companySorted.close();
    }

    std::cout << "App finished!\n";
    app.quit();
    return app.exec();
}