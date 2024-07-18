#include <iostream>
#include <fstream>

#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QThread>

#include <LinkedApp.h>
LinkedApp::LinkedApp(QObject *parent) : QObject(parent)
{
    std::cout << "LinkedApp ctr\n";
    QObject::connect(&networkManager, &QNetworkAccessManager::finished, this, &LinkedApp::processHTML);
}

void LinkedApp::removeSpaces(QString &str)
{
    if (str.size() < 2)
        return;
    qsizetype index = 1;
    while (str.size() > index)
    {
        if ((str[index] == ' ' && str[index - 1] == ' ') ||
            (str[index] == '\n' && str[index - 1] == '\n') ||
            (str[index] == ' ' && str[index - 1] == '\n'))
        {
            str.remove(index, 1);
            continue;
        }
        ++index;
    }
}

void LinkedApp::sendSearchRequest(QString req, QString location)
{
    vacancies.clear();
    // use req and location once, but for now just:
    currrentRequestType = RequestType::title;
    sendRequest(titleLink.arg(vacancies.size()));
    std::cout << "sendSearchRequest finished\n";
}

void LinkedApp::sendRequest(QString link)
{
    // std::cout << "sendRequest START\n";
    // QFile test("response.html");
    // QFile test2("output");
    // if (!test.open(QIODeviceBase::ReadOnly))
    // {
    //     std::cout << " couldn't open file" << test2.fileName().toStdString();
    //     return;
    // }
    // if (!test2.open(QIODeviceBase::WriteOnly))
    // {
    //     std::cout << " couldn't open file" << test.fileName().toStdString();
    //     return;
    // }
    // QTextStream test2Stream(&test2);
    // QString fileString = test.readAll();
    // fileString.remove("</li>");
    // removeSpaces(fileString);
    // auto data = fileString.split("<li>");
    // LinkedVacancy vacancy;
    // for (auto n = 1; n < data.size(); ++n)
    // {
    //     vacancy.load(data[n]);
    //     test2Stream << "!!! " << n << " !!!\n";
    //     vacancy.print(test2Stream);
    //     test2Stream << "\n";
    // }
    // test2.close();
    // std::cout << "sendRequest END\n";

    QString info;
    if (currrentRequestType == RequestType::title)
        info = QString::number(vacancies.size()) + " title";
    else
        info = QString::number(descriptionsFinished) + "/" + QString::number(vacancies.size()) + " description";

    request.setUrl(link);

    QThread::msleep(500);
    std::cout << "about to send " << info.toStdString() << " request: " << request.url().toDisplayString().toStdString() << "\n";
    networkManager.get(request);
}

void LinkedApp::processHTML(QNetworkReply *reply)
{
    auto errorState = reply->error();
    if (errorState != QNetworkReply::NoError)
    {
        std::cout << "Reply error " << errorState << "\n";
        if (currrentRequestType == RequestType::title)
            sendRequest(titleLink.arg(vacancies.size()));
        else
            sendRequest(descriptionLink);
        return;
    }
    if (currrentRequestType == RequestType::title)
        processTitle(reply);
    else
        processDescription(reply);
}

void LinkedApp::processTitle(QNetworkReply *reply)
{
    QString replyString = parseTitle(reply->readAll());

    auto data = replyString.split("<li>");
    if (data.size() < 2)
    {
        if (descriptionsFinished < vacancies.size())
        {
            currrentRequestType = RequestType::description;
            descriptionLink = vacancies[descriptionsFinished].link;
            sendRequest(descriptionLink);
        }
        else
            std::cout << "Error: descriptionsFinished >= vacancies.size() " << descriptionsFinished << " >= " << vacancies.size() << "\n";
        return;
    }

    QDir titlesFolder("titles");
    if (!titlesFolder.exists())
        titlesFolder.mkpath(".");

    QString fileName = "%1_title_response.html";
    QFile out(titlesFolder.absoluteFilePath(fileName.arg(vacancies.size(), 5, 10, QChar('0'))));
    if (out.open(QIODeviceBase::WriteOnly))
    {
        QTextStream outStream(&out);
        outStream << replyString;
        out.close();
    }
    else
        std::cout << "couldn't open file " << out.fileName().toStdString() << "\n";

    for (auto n = 1; n < data.size(); ++n)
    {
        currentVacancy.load(data[n]);
        vacancies << currentVacancy;
        currentVacancy.clear();
    }
    sendRequest(titleLink.arg(vacancies.size()));
}

void LinkedApp::processDescription(QNetworkReply *reply)
{
    // some actions maybe...

    QString replyString = parseDescription(reply->readAll());

    QDir descriptionFolder("description");
    if (!descriptionFolder.exists())
        descriptionFolder.mkpath(".");
    QString fileName = "%1_description_response.html";

    QFile out(descriptionFolder.absoluteFilePath(fileName.arg(descriptionsFinished, 5, 10, QChar('0'))));
    if (out.open(QIODeviceBase::WriteOnly))
    {
        QTextStream outStream(&out);
        outStream << replyString;
        out.close();
    }
    else
        std::cout << "couldn't open file\n";

    vacancies[descriptionsFinished].loadDescription(replyString);
    ++descriptionsFinished;
    if (descriptionsFinished >= vacancies.size())
    {
        finishedRequest();
        QCoreApplication::quit();
        return;
    }
    descriptionLink = vacancies[descriptionsFinished].link;
    sendRequest(descriptionLink);
}

QString LinkedApp::parseTitle(QString str)
{
    str.remove("</li>");
    removeSpaces(str);
    return str;
}

QString LinkedApp::parseDescription(QString str)
{
    removeSpaces(str);
    return str;
}

void LinkedApp::finishedRequest()
{
    QDir sortedDir("sorted");
    if (!sortedDir.exists())
        sortedDir.mkpath(".");

    sortByRelevance("C++");

    QFile relevanceSorted(sortedDir.absoluteFilePath("relevanceSorted"));
    if (relevanceSorted.open(QIODeviceBase::WriteOnly))
    {
        QTextStream relevanceSortedStream(&relevanceSorted);
        for (const auto &vacancy : vacancies)
        {
            vacancy.print(relevanceSortedStream);
            relevanceSortedStream << "\n";
        }
        relevanceSorted.close();
    }

    sortByTime();

    QFile timeSorted(sortedDir.absoluteFilePath("timeSorted"));
    if (timeSorted.open(QIODeviceBase::WriteOnly))
    {
        QTextStream timeSortedStream(&timeSorted);
        for (const auto &vacancy : vacancies)
        {
            vacancy.print(timeSortedStream);
            timeSortedStream << "\n";
        }
        timeSorted.close();
    }

    sortByTitle();

    QFile titleSorted(sortedDir.absoluteFilePath("titleSorted"));
    if (titleSorted.open(QIODeviceBase::WriteOnly))
    {
        QTextStream titleSortedStream(&titleSorted);
        for (const auto &vacancy : vacancies)
        {
            vacancy.print(titleSortedStream);
            titleSortedStream << "\n";
        }
        titleSorted.close();
    }

    sortByCompany();

    QFile companySorted(sortedDir.absoluteFilePath("companySorted"));
    if (companySorted.open(QIODeviceBase::WriteOnly))
    {
        QTextStream companySortedStream(&companySorted);
        for (const auto &vacancy : vacancies)
        {
            vacancy.print(companySortedStream);
            companySortedStream << "\n";
        }
        companySorted.close();
    }
}

QString LinkedApp::loadTitleFromFile(const QString &path)
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

QString LinkedApp::loadDescriptionFromFile(const QString &path)
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

const QList<LinkedVacancy> &LinkedApp::sortByTime()
{
    auto compareByTime = [](const auto &left, const auto &right)
    {
        return left.date > right.date;
    };
    std::sort(vacancies.begin(), vacancies.end(), compareByTime);
    return vacancies;
}

const QList<LinkedVacancy> &LinkedApp::sortByTitle()
{
    auto compareByTitle = [](const auto &left, const auto &right)
    {
        return left.title < right.title;
    };
    std::sort(vacancies.begin(), vacancies.end(), compareByTitle);
    return vacancies;
}

const QList<LinkedVacancy> &LinkedApp::sortByCompany()
{
    auto compareByCompany = [](const auto &left, const auto &right)
    {
        return left.companyName < right.companyName;
    };

    std::sort(vacancies.begin(), vacancies.end(), compareByCompany);
    return vacancies;
}

const QList<LinkedVacancy> &LinkedApp::sortByRelevance(const QString &str)
{
    auto compareByRelevance = [&](const auto &left, const auto &right)
    {
        return left.count(str) > right.count(str);
    };

    std::sort(vacancies.begin(), vacancies.end(), compareByRelevance);
    return vacancies;
}

const QList<LinkedVacancy> &LinkedApp::getVacancies()
{
    return vacancies;
}