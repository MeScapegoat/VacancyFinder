#include <iostream>
#include <fstream>

#include <QCoreApplication>
#include <QFile>
#include <QThread>

#include <LinkedApp.h>
LinkedApp::LinkedApp(QObject *parent) : QObject(parent)
{
    std::cout << "LinkedApp ctr\n";
    QObject::connect(&networkManager, &QNetworkAccessManager::finished, this, &LinkedApp::parseHTML);
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
    QString link = titleLink;
    currrentRequestType = RequestType::title;
    sendRequest(link.arg(vacancies.size()));
    sortByRelevance(req);
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

    lastLink = link;
    QThread::msleep(500);
    request.setUrl(link.arg(vacancies.size()));
    std::cout << "about to send " << (currrentRequestType == RequestType::title ? "title" : "description") << " request: " << request.url().toDisplayString().toStdString() << "\n";
    networkManager.get(request);
}

const QList<LinkedVacancy> &LinkedApp::getVacancies()
{
    return vacancies;
}

const QList<LinkedVacancy> &LinkedApp::sortByTime()
{
    auto compareByTime = [](const auto& left, const auto& right)
    {
        return left.date > right.date; 
    };
    std::sort(vacancies.begin(), vacancies.end(), compareByTime);
    return vacancies;
}

const QList<LinkedVacancy> &LinkedApp::sortByTitle()
{
    auto compareByTitle = [](const auto& left, const auto& right)
    {
        return left.title > right.title; 
    };
    std::sort(vacancies.begin(), vacancies.end(), compareByTitle);
    return vacancies;
}

const QList<LinkedVacancy> &LinkedApp::sortByCompany()
{
    auto compareByCompany = [](const auto& left, const auto& right)
    {
        return left.companyName > right.companyName;
    };

    std::sort(vacancies.begin(), vacancies.end(), compareByCompany);
    return vacancies;
}

const QList<LinkedVacancy> &LinkedApp::sortByRelevance(const QString& str)
{
    auto compareByRelevance = [&](const auto& left, const auto& right)
    {
        return left.count(str) > right.count(str); 
    };

    std::sort(vacancies.begin(), vacancies.end(), compareByRelevance);
    return vacancies;
}

void LinkedApp::parseHTML(QNetworkReply *reply)
{
    auto errorState = reply->error();
    if (errorState != QNetworkReply::NoError)
    {
        std::cout << "Reply error " << errorState << "\n";
        if (currrentRequestType == RequestType::title)
            sendRequest(titleLink.arg(vacancies.size()));
        else
            sendRequest(lastLink.arg(vacancies.size()));
        return;
    }
    if (currrentRequestType == RequestType::title)
        parseTitle(reply);
    else
        parseDescription(reply);
}

void LinkedApp::parseTitle(QNetworkReply *reply)
{
    QFile out(QString("response_%1_title.html").arg(vacancies.size()));
    if (!out.open(QIODeviceBase::WriteOnly))
        std::cout << "couldn't open file\n";
    QTextStream outStream(&out);

    QString replyString = reply->readAll();
    replyString.remove("</li>");
    removeSpaces(replyString);

    outStream << replyString;
    out.close();

    auto data = replyString.split("<li>");
    if (data.size() < 2)
        return;

    for (auto n = 1; n < data.size(); ++n)
    {
        currentVacancy.load(data[n]);
        currrentRequestType = RequestType::description;
        sendRequest(currentVacancy.link);
        vacancies << currentVacancy;
        currentVacancy.clear();
    }
    currrentRequestType = RequestType::title;
    sendRequest(titleLink.arg(vacancies.size()));
}

void LinkedApp::parseDescription(QNetworkReply *reply)
{
    QFile out(QString("response_%1_description.html").arg(vacancies.size()));
    if (!out.open(QIODeviceBase::WriteOnly))
        std::cout << "couldn't open file\n";
    QTextStream outStream(&out);

    // some actions maybe...

    QString replyString = reply->readAll();
    outStream << replyString;
    out.close();
    currentVacancy.description = out.fileName();
}