// std
// Qt
#include <QThread>
#include <QLayout>
// custom
#include "GUI.h"

namespace Linked
{
    Q_LOGGING_CATEGORY(LinkedGUI, "gui");

    GUI::GUI(SharedContext *sharedContext) : context(sharedContext), mainWindow_(new MainWindow), progressDialog_(new ProgressDialog(mainWindow_, context)) {}

    MainWindow *GUI::mainWindow()
    {
        return mainWindow_;
    }

    ProgressDialog *GUI::progressDialog()
    {
        return progressDialog_;
    }

} // namespace Linked