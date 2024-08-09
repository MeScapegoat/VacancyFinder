#pragma once
// std

// Qt
#include <QObject>

// custom
#include "MainWindow.h"

namespace Linked
{
    Q_DECLARE_LOGGING_CATEGORY(LinkedGUI);

    class GUI : public QObject
    {
        Q_OBJECT

    public:
        GUI() = default;
        ~GUI() = default;

    public:
        MainWindow *mainWindow();

    private:
        MainWindow mainWindow_;
    };

} // namespace Linked