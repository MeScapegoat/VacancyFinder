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

void LinkedApp::sendRequest()
{
    std::cout << "sendRequest START\n";
    QFile test("response.html");
    QFile test2("output");
    if (!test.open(QIODeviceBase::ReadOnly))
    {
        std::cout << " couldn't open file" << test2.fileName().toStdString();
        return;
    }
    if (!test2.open(QIODeviceBase::WriteOnly))
    {
        std::cout << " couldn't open file" << test.fileName().toStdString();
        return;
    }
    QTextStream test2Stream(&test2);
    QString fileString = test.readAll();
    fileString.remove("</li>");
    removeSpaces(fileString);
    auto data = fileString.split("<li>");
    LinkedVacancy vacancy;
    for (auto n = 1; n < data.size(); ++n)
    {
        vacancy.load(data[n]);
        test2Stream << "!!! " << n << " !!!\n";
        vacancy.print(test2Stream);
        test2Stream << "\n";
    }
    test2.close();
    std::cout << "sendRequest END\n";

    // QThread::msleep(300);
    // if (vacanciesProcessed)
    // {
    //     std::cout << "Finished!\n";
    //     for (auto n = 0; n < vacancies.size(); ++n)
    //     {
    //         std::cout << n << ". " << vacancies[n].link.toStdString() << "\n";
    //     }
    //     std::cout << "Unique links:" << links.size() << "\n";
    //     std::cout << "Unique names:" << names.size() << "\n";
    //     QCoreApplication::quit();
    //     return;
    // }
    // request.setUrl(link.arg(vacancies.size()));
    // std::cout << "about to send request: " << request.url().toDisplayString().toStdString() << "\n";
    // networkManager.get(request);
}

void LinkedApp::getVacancies()
{
}

void LinkedApp::parseHTML(QNetworkReply *reply)
{
    auto errorState = reply->error();
    if (errorState != QNetworkReply::NoError)
    {
        std::cout << "Reply error " << errorState << "\n";
        sendRequest();
        return;
    }
    QFile out(QString("response%1.html").arg(vacancies.size()));
    if (!out.open(QIODeviceBase::WriteOnly))
        std::cout << "couldn't open file\n";
    QTextStream outStream(&out);
    QString replyString = reply->readAll();
    replyString.remove("</li>");
    removeSpaces(replyString);
    outStream << replyString;
    auto data = replyString.split("<li>");
    if (data.size() < 2)
    {
        vacanciesProcessed = true;
        sendRequest();
        return;
    }
    for (auto n = 1; n < data.size(); ++n)
    {
        LinkedVacancy vacancy(data[n]);
        vacancies << vacancy;
        links << vacancy.link;
        names << vacancy.title;
    }
    sendRequest();
}