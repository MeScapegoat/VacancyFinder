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
        inSearchMessage_ = new QMessageBox(QMessageBox::Information,
                                           "Searching...",
                                           "Please wait for search to complete", QMessageBox::NoButton, mainWindow());
        inSearchMessage_->setStandardButtons(QMessageBox::NoButton);
    }

    MainWindow *GUI::mainWindow()
    {
        return &mainWindow_;
    }

    QMessageBox *GUI::inSearchMessage()
    {
        return inSearchMessage_;
    }

} // namespace Linked