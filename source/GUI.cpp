// std
// Qt
#include <QThread>
// custom
#include "GUI.h"

namespace Linked
{
    Q_LOGGING_CATEGORY(LinkedGUI, "gui");

    MainWindow *GUI::mainWindow()
    {
        return &mainWindow_;
    }

} // namespace Linked