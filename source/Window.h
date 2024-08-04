#pragma once
// std

// Qt
#include <QMainWindow>
#include <QLoggingCategory>
#include <QListView>

// custom

namespace Linked
{
    Q_DECLARE_LOGGING_CATEGORY(LinkedGUIWindow);

    class Window : public QMainWindow
    {
    public:
        Window();
        ~Window();
    
    public slots:
        void test();

    protected:
        void closeEvent(QCloseEvent *) override;
        QListView* vacanciesList = nullptr;
    };

} // namespace Linked