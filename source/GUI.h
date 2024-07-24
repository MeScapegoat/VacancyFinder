#pragma once
// std

// Qt
#include <QObject>

// custom
#include "Window.h"

namespace Linked
{
    Q_DECLARE_LOGGING_CATEGORY(LinkedGUI);

    class GUI : public QObject
    {
        Q_OBJECT

    public:
        GUI();
        ~GUI();

    public:
    private:
        Window window;
    };

} // namespace Linked