// std
// Qt
#include <QCloseEvent>
#include <QApplication>
#include <QThread>

// custom
#include "Window.h"

namespace Linked
{
    Q_LOGGING_CATEGORY(LinkedGUIWindow, "gui.window");

    Window::Window()
    {
        qCDebug(LinkedGUIWindow) << QThread::currentThread();
        auto screenSize = QApplication::screens()[0]->size() / 2;
        qsizetype x, y, w, h;
        w = screenSize.width();
        h = screenSize.height();
        x = w / 2;
        y = h / 2;
        
        setGeometry(x, y, w, h);
        show();
    }

    Window::~Window()
    {
        qCDebug(LinkedGUIWindow) << QThread::currentThread();
    }

    void Window::closeEvent(QCloseEvent *event)
    {
        qCDebug(LinkedGUIWindow) << QThread::currentThread();
        qCInfo(LinkedGUIWindow) << "closeEvent";
        event->accept();
    }

} // namespace Linked