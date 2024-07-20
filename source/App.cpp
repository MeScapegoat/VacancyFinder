#include <iostream>
#include <fstream>

#include <QCoreApplication>
#include <QFile>
#include <QUrlQuery>
#include <QDir>
#include <QThread>

#include <App.h>
namespace Linked
{
    App::App()
    {
        qInfo() << "App ctr";
        qInfo() << "QObject::connect(&networkManager, &NetworkManager::finished, &parser, &HTMLparser::processHTML);";
        QObject::connect(&networkManager, &NetworkManager::finished, &parser, &HTMLparser::processHTML);
        qInfo() << "QObject::connect(this, &App::sendRequest, &networkManager, &NetworkManager::sendRequest);";
        QObject::connect(this, &App::sendRequest, &networkManager, &NetworkManager::sendRequest);
        qInfo() << "QObject::connect(this, &App::titleReady, &parser, &HTMLparser::titleReady);";
        QObject::connect(&parser, &HTMLparser::titleReady, this, &App::titleReady);
        qInfo() << "QObject::connect(this, &App::descrReady, &parser, &HTMLparser::descrReady);";
        QObject::connect(&parser, &HTMLparser::descrReady, this, &App::descrReady);
    }

    void App::sendSearchRequest(QString keyword, QString location)
    {
        qInfo() << "sendSearchRequest";

        vacancies.clear();

        QUrl url(baseUrl);
        QUrlQuery query;
        query.addQueryItem("keyword", QUrl::toPercentEncoding(keyword));
        query.addQueryItem("location", QUrl::toPercentEncoding(location));
        query.addQueryItem("start", QUrl::toPercentEncoding("0"));
        url.setQuery(query);
        emit sendRequest(url);

        // currrentRequestType = RequestType::title;
        // sendRequest(titleLink.arg(vacancies.size()));
        // qInfo() << "sendSearchRequest finished\n";
    }

    // void App::sendRequest(QString link)
    // {
    //     // qInfo() << "sendRequest START\n";
    //     // QFile test("response.html");
    //     // QFile test2("output");
    //     // if (!test.open(QIODeviceBase::ReadOnly))
    //     // {
    //     //     qInfo() << " couldn't open file" << test2.fileName().toStdString();
    //     //     return;
    //     // }
    //     // if (!test2.open(QIODeviceBase::WriteOnly))
    //     // {
    //     //     qInfo() << " couldn't open file" << test.fileName().toStdString();
    //     //     return;
    //     // }
    //     // QTextStream test2Stream(&test2);
    //     // QString fileString = test.readAll();
    //     // fileString.remove("</li>");
    //     // removeSpaces(fileString);
    //     // auto data = fileString.split("<li>");
    //     // Vacancy vacancy;
    //     // for (auto n = 1; n < data.size(); ++n)
    //     // {
    //     //     vacancy.load(data[n]);
    //     //     test2Stream << "!!! " << n << " !!!\n";
    //     //     vacancy.print(test2Stream);
    //     //     test2Stream << "\n";
    //     // }
    //     // test2.close();
    //     // qInfo() << "sendRequest END\n";

    //     QString info;
    //     if (currrentRequestType == RequestType::title)
    //         info = QString::number(vacancies.size()) + " title";
    //     else
    //         info = QString::number(descriptionsFinished + 1) + "/" + QString::number(vacancies.size()) + " description";

    //     request.setUrl(link);

    //     QThread::msleep(500);
    //     qInfo() << "about to send " << info.toStdString() << " request: " << request.url().toDisplayString().toStdString() << "\n";
    //     networkManager.get(request);
    // }

    void App::titleReady(Vacancy vacancy)
    {
        QString link = vacancy.link;
        vacancies.push_back(std::move(vacancy));

        emit sendRequest(link);
    }

    void App::descrReady(Vacancy vacancy)
    {
        qInfo() << "descrReady" << "vacancies.size():" << vacancies.size();
    }

    // void App::finishedRequest()
    // {
    //     qInfo() << "finishedRequest";

    //     QDir sortedDir("sorted");
    //     if (!sortedDir.exists())
    //         sortedDir.mkpath(".");

    //     sortByRelevance("C++");

    //     QFile relevanceSorted(sortedDir.absoluteFilePath("relevanceSorted"));
    //     if (relevanceSorted.open(QIODeviceBase::WriteOnly))
    //     {
    //         QTextStream relevanceSortedStream(&relevanceSorted);
    //         for (const auto &vacancy : vacancies)
    //         {
    //             vacancy.print(relevanceSortedStream);
    //             relevanceSortedStream << "\n";
    //         }
    //         relevanceSorted.close();
    //     }

    //     sortByTime();

    //     QFile timeSorted(sortedDir.absoluteFilePath("timeSorted"));
    //     if (timeSorted.open(QIODeviceBase::WriteOnly))
    //     {
    //         QTextStream timeSortedStream(&timeSorted);
    //         for (const auto &vacancy : vacancies)
    //         {
    //             vacancy.print(timeSortedStream);
    //             timeSortedStream << "\n";
    //         }
    //         timeSorted.close();
    //     }

    //     sortByTitle();

    //     QFile titleSorted(sortedDir.absoluteFilePath("titleSorted"));
    //     if (titleSorted.open(QIODeviceBase::WriteOnly))
    //     {
    //         QTextStream titleSortedStream(&titleSorted);
    //         for (const auto &vacancy : vacancies)
    //         {
    //             vacancy.print(titleSortedStream);
    //             titleSortedStream << "\n";
    //         }
    //         titleSorted.close();
    //     }

    //     sortByCompany();

    //     QFile companySorted(sortedDir.absoluteFilePath("companySorted"));
    //     if (companySorted.open(QIODeviceBase::WriteOnly))
    //     {
    //         QTextStream companySortedStream(&companySorted);
    //         for (const auto &vacancy : vacancies)
    //         {
    //             vacancy.print(companySortedStream);
    //             companySortedStream << "\n";
    //         }
    //         companySorted.close();
    //     }

    //     sortByLevel();

    //     QFile levelSorted(sortedDir.absoluteFilePath("levelSorted"));
    //     if (levelSorted.open(QIODeviceBase::WriteOnly))
    //     {
    //         QTextStream levelSortedStream(&levelSorted);
    //         for (const auto &vacancy : vacancies)
    //         {
    //             vacancy.print(levelSortedStream);
    //             levelSortedStream << "\n";
    //         }
    //         levelSorted.close();
    //     }
    // }

    QString App::loadTitleFromFile(const QString &path)
    {
        QFile file(path);
        QString ret;
        if (file.open(QIODeviceBase::ReadOnly))
        {
            ret = file.readAll();
            file.close();
        }
        else
            qInfo() << "couldn't open " << path;
        return ret;
    }

    QString App::loadDescriptionFromFile(const QString &path)
    {
        QFile file(path);
        QString ret;
        if (file.open(QIODeviceBase::ReadOnly))
        {
            ret = file.readAll();
            file.close();
        }
        else
            qInfo() << "couldn't open " << path;
        return ret;
    }

    const QList<Vacancy> &App::sortByTime()
    {
        auto compareByTime = [](const auto &left, const auto &right)
        {
            return left.date > right.date;
        };
        std::sort(vacancies.begin(), vacancies.end(), compareByTime);
        return vacancies;
    }

    const QList<Vacancy> &App::sortByTitle()
    {
        auto compareByTitle = [](const auto &left, const auto &right)
        {
            return left.title < right.title;
        };
        std::sort(vacancies.begin(), vacancies.end(), compareByTitle);
        return vacancies;
    }

    const QList<Vacancy> &App::sortByCompany()
    {
        auto compareByCompany = [](const auto &left, const auto &right)
        {
            return left.companyName < right.companyName;
        };

        std::sort(vacancies.begin(), vacancies.end(), compareByCompany);
        return vacancies;
    }

    const QList<Vacancy> &App::sortByRelevance(const QString &str)
    {
        auto compareByRelevance = [&](const auto &left, const auto &right)
        {
            return left.count(str) > right.count(str);
        };

        std::sort(vacancies.begin(), vacancies.end(), compareByRelevance);
        return vacancies;
    }

    const QList<Vacancy> &App::sortByLevel()
    {
        auto compareByRelevance = [&](const auto &left, const auto &right)
        {
            return left.level < right.level;
        };

        std::sort(vacancies.begin(), vacancies.end(), compareByRelevance);
        return vacancies;
    }

    const QList<Vacancy> &App::getVacancies()
    {
        return vacancies;
    }

} // namespace Linked