#pragma once

namespace Linked
{
    class App;
}

class MainWindow;

class SharedContext
{
public:
    const Linked::App* app = nullptr;
    const MainWindow* mainWindow = nullptr;
};