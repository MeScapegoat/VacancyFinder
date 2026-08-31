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
#include "../model/VacancyBrief.h"
#include "../model/VacancyDetails.h"

namespace Linked
{
    class ProgressDialog : public QDialog
    {
    public:
        ProgressDialog(QWidget *parent, SharedContext *context);
        ~ProgressDialog() = default;

    public slots:
        void updateBrief(const VacancyBrief& brief);
        void updateDetails(const VacancyDetails& details);
        void updateError(QNetworkReply::NetworkError);
        void start(const unsigned& maxVacancies);

    public:
        QPushButton& stopButton;

    private:
        SharedContext *context = nullptr;

    private:
        QLabel& briefLabel;
        QProgressBar& briefsProgressBar;


        QLabel& detailsLabel;
        QProgressBar& allDetailsProgressBar;

        QLabel& errorLabel;
    };
} // namespace Linked