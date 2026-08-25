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

        infoLabel_->setAlignment(Qt::AlignCenter);
        errorLabel_->setAlignment(Qt::AlignCenter);

        layout->addWidget(infoLabel_);
        layout->addWidget(errorLabel_);
        layout->addWidget(progressBar_);
        layout->addWidget(stopButton_);
        
        setFixedSize(500, 200);

        setLayout(layout);
    }

    void ProgressDialog::updateInfo(const QUrl& link)
    {
        const Vacancy& vacancy = context->app->getVacancies().value(link);
        infoLabel_->setText(infoPattern
                                .arg(0)
                                .arg(vacancy.title)
                                .arg(vacancy.companyName));
        progressBar_->setValue(progressBar_->value() + 1);
    }

    void ProgressDialog::updateError(QNetworkReply::NetworkError err)
    {
        qCDebug(LinkedGUI) << "update error:" << err;

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
        progressBar_->setValue(0);
        infoLabel_->setText("Processing vacancies...");
        errorLabel_->setText("");

        exec();
    }
} // namespace Linked