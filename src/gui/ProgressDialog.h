#pragma once

// std

// Qt
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QNetworkReply>

// custom
#include "../core/SharedContext.h"
#include "../core/Vacancy.h"

namespace Linked
{
    class ProgressDialog : public QDialog
    {
    public:
        ProgressDialog(QWidget *parent, SharedContext *context);
        ~ProgressDialog() = default;

    public:
        QPushButton *stopButton() { return stopButton_; }

    public slots:
        void updateInfo(const QUrl& link);
        void updateError(QNetworkReply::NetworkError);
        void start();

    private:
        SharedContext *context = nullptr;

    private:
        QLabel *infoLabel_ = nullptr;
        QLabel *errorLabel_ = nullptr;
        QPushButton *stopButton_ = nullptr;
        QProgressBar *progressBar_ = nullptr;

        QString infoPattern = "Processing vacancy #%1\n\n%2\n%3";
    };
} // namespace Linked