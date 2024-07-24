// std
// Qt
#include <QThread>
// custom
#include "GUI.h"

namespace Linked
{
    Q_LOGGING_CATEGORY(LinkedGUI, "gui");

    GUI::GUI()
    {
        qCDebug(LinkedGUI) << QThread::currentThread();
    }

    GUI::~GUI()
    {
        qCDebug(LinkedGUI) << QThread::currentThread();
    }

} // namespace Linked