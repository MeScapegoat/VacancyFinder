#pragma once
// std

// Qt
#include <QObject>
#include <QMessageBox>

// custom
#include "../core/SharedContext.h"
#include "MainWindow.h"
#include "ProgressDialog.h"

namespace Linked
{
    Q_DECLARE_LOGGING_CATEGORY(LinkedGUI);

    class ProgressDialog;
    class MainWindow;

    class GUI : public QObject
    {
        Q_OBJECT

    public:
        GUI(SharedContext *context);
        ~GUI() = default;

    public:
        MainWindow *mainWindow();

        ProgressDialog *progressDialog();

    private:
        SharedContext *context;

    private:
        MainWindow* mainWindow_;

        ProgressDialog* progressDialog_;
    };

} // namespace Linked