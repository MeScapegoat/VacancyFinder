#include <iostream>

#include <QCoreApplication>

#include <LinkedApp.h>
LinkedApp::LinkedApp(QObject* parent) : QObject(parent) 
{
    std::cout << "LinkedApp ctr\n";
}

void LinkedApp::sendRequest()
{
    std::cout << "sendRequest start\n";
    request.setUrl(linkInit);
    std::cout << "about to send request: " << request.url().toDisplayString().toStdString() << "\n";
    reply = networkManager.get(request);
    QObject::connect(reply, QNetworkReply::finished, this, parseHTML);
    std::cout << "sendRequest end\n";
}

void LinkedApp::getVacancies()
{

}

void LinkedApp::parseHTML()
{
    std::cout << "reply finished\n";
    std::cout << reply->readAll().toStdString() << "\n";
    QCoreApplication::quit();
}