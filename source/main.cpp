// std
#include <iostream>
#include <fstream>

// Qt
#include <QCoreApplication> // TODO swap to gui

// custom
#include <LinkedApp.h>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv); // TODO swap to gui
    std::cout << "App started!\n";
    LinkedApp linkedApp(&app);
    linkedApp.sendSearchRequest();
    std::cout << "App finished!\n";
    app.quit();
    return app.exec();
}