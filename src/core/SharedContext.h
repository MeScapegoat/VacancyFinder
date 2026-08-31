#pragma once

namespace Linked
{
    class App;
    class MainWindow;
    
    class SharedContext
    {
    public:
        const App *app = nullptr;
        const MainWindow *mainWindow = nullptr;
    };

} // namespace Linked