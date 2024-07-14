#include <iostream>
#include <fstream>

#include <QCoreApplication>
#include <QFile>

#include <LinkedApp.h>
LinkedApp::LinkedApp(QObject *parent) : QObject(parent)
{
    std::cout << "LinkedApp ctr\n";
    QObject::connect(&networkManager, QNetworkAccessManager::finished, this, parseHTML);
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
    std::cout << "sendRequest start\n";
    request.setUrl(link.arg(vacancies.size()));
    std::cout << "about to send request: " << request.url().toDisplayString().toStdString() << "\n";
    networkManager.get(request);
    std::cout << "sendRequest end\n";
}

void LinkedApp::getVacancies()
{
}

void LinkedApp::parseHTML(QNetworkReply *reply)
{
    std::cout << "reply finished\n";
    auto errorState = reply->error();
    if (errorState != QNetworkReply::NoError)
    {
        std::cout << "Reply error " << errorState << "\n";
        QCoreApplication::quit();
    }
    QFile out(QString("response%1.html").arg(vacancies.size()));
    if (!out.open(QIODeviceBase::WriteOnly))
        std::cout << "couldn't open file";
    QTextStream outStream(&out);
    QString replyString = reply->readAll();
    replyString.remove("</li>");
    removeSpaces(replyString);
    outStream << replyString;
    auto data = replyString.split("<li>");
    for (auto n = 1; n < data.size(); ++n)
        vacancies << data[n];
    sendRequest();
}