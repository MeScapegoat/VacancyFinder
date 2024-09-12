#pragma once
// std

// Qt
#include <QObject>
#include <QMessageBox>

// custom
#include "MainWindow.h"

namespace Linked
{
    Q_DECLARE_LOGGING_CATEGORY(LinkedGUI);

    class GUI : public QObject
    {
        Q_OBJECT

    public:
        GUI();
        ~GUI() = default;

    public:
        MainWindow *mainWindow();
        QMessageBox* inSearchMessage();

    private:
        MainWindow mainWindow_;
        QMessageBox *inSearchMessage_;
    };

} // namespace Linked