#pragma once
// std

// Qt
#include <QMainWindow>
#include <QLoggingCategory>

// custom

namespace Linked
{
    Q_DECLARE_LOGGING_CATEGORY(LinkedGUIWindow);

    class Window : public QMainWindow
    {
    public:
        Window();
        ~Window();

    protected:
        void closeEvent(QCloseEvent *) override;
    };

} // namespace Linked