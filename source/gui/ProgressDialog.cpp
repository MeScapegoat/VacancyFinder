// std

// Qt
#include <QVBoxLayout>

// custom
#include "ProgressDialog.h"

namespace Linked
{
    ProgressDialog::ProgressDialog(QWidget *parent, SharedContext *sharedContext) : QDialog(parent), context(sharedContext)
    {
        setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint);

        QVBoxLayout *layout = new QVBoxLayout();
        infoLabel_ = new QLabel();
        errorLabel_ = new QLabel();
        progressBar_ = new QProgressBar;
        stopButton_ = new QPushButton("Stop Search");

        layout->addWidget(infoLabel_);
        layout->addWidget(errorLabel_);
        layout->addWidget(progressBar_);
        layout->addWidget(stopButton_);

        setLayout(layout);
    }

    void ProgressDialog::updateInfo(qsizetype id)
    {
        auto vacancy = context->app->getVacancies().at(id);
        infoLabel_->setText(infoPattern
                                .arg(id)
                                .arg(vacancy.title)
                                .arg(vacancy.companyName));
        progressBar_->setValue(progressBar_->value() + 1);
    }

    void ProgressDialog::updateError(QNetworkReply::NetworkError err)
    {
        QString errorText;
        QString errorTime = "\nTime of error: " + QTime::currentTime().toString();
        switch (err)
        {
        case 0:
            errorText = "No error occurred";
            break;
        case 1:
            errorText = "Error: Connection Refused" + errorTime;
            break;
        case 299:
            errorText = "Error: Unknown Content" + errorTime;
            break;
        default:
            errorText = "Error code: " + QString::number(err) + errorTime;
            break;
        }

        errorLabel_->setText(errorText);
    }

    void ProgressDialog::start()
    {
        progressBar_->setRange(0, context->mainWindow->maxVacancies());
        infoLabel_->setText("Processing vacancies...");
        errorLabel_->setText("");

        exec();
    }
} // namespace Linked