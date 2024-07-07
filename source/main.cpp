#include <iostream>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QNetworkReply>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    std::cout << "App started!\n";
    QNetworkAccessManager networkManager;
    QNetworkRequest request;
    request.setUrl(QUrl("https://www.google.com/"));
    QNetworkReply *reply = networkManager.get(request);
    if (reply->isRunning())
        std::cout << "reply is running\n";
    else
        std::cout << "reply is not running\n";
        
    if (reply->isFinished())
        std::cout << "reply is finished\n";
    else
        std::cout << "reply is not finished\n";
    while (reply->isRunning() && !reply->isFinished())
    {
        // std::cout << "?\n";
    }
    std::cout << "App finished!\n";

    return app.exec();
}