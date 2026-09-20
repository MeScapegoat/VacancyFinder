// std

// Qt
#include <QVBoxLayout>

// custom
#include "ProgressDialog.h"

ProgressDialog::ProgressDialog(QWidget* parent) :
    QDialog(parent),
    briefLabel(*new QLabel),
    briefsProgressBar(*new QProgressBar),
    detailsLabel(*new QLabel),
    allDetailsProgressBar(*new QProgressBar),
    errorLabel(*new QLabel),
    stopButton(*new QPushButton)
{
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint);

    QVBoxLayout& layout = *new QVBoxLayout();

    stopButton.setText("Stop Search");

    layout.addWidget(&briefLabel);
    layout.addWidget(&briefsProgressBar);

    layout.addWidget(&detailsLabel);
    layout.addWidget(&allDetailsProgressBar);

    layout.addWidget(&errorLabel);

    layout.addWidget(&stopButton);

    setMinimumSize(500, 200);

    setLayout(&layout);

    QObject::connect(&stopButton, &QPushButton::released, this, &ProgressDialog::accept);
}

void ProgressDialog::updateBrief(const Vacancy& vacancy)
{
    static constexpr const char* briefPattern = "Processed brief: #%1, %2 by %3";

    briefLabel.setText(QString(briefPattern)
                       .arg(vacancy.id)
                       .arg(vacancy.title)
                       .arg(vacancy.companyName));

    briefsProgressBar.setValue(vacancy.id);
}

void ProgressDialog::updateDetails(const Vacancy& vacancy)
{
    static constexpr const char* detailsPattern = "Processed details: #%1";

    detailsLabel.setText(QString(detailsPattern).arg(vacancy.id));

    allDetailsProgressBar.setValue(vacancy.id);
}

void ProgressDialog::updateError(QNetworkReply::NetworkError err)
{
    QString errorText;
    const QString errorTime = "\nTime of error: " + QTime::currentTime().toString();
    switch (err)
    {
        case QNetworkReply::NetworkError::NoError:
        errorText = "No error occurred";
        break;
        case QNetworkReply::NetworkError::ConnectionRefusedError:
        errorText = "Error: Connection Refused" + errorTime;
        break;
        case QNetworkReply::NetworkError::UnknownContentError:
        errorText = "Error: Unknown Content" + errorTime;
        break;
        default:
        errorText = "Error code: " + QString::number(err) + errorTime;
        break;
    }

    errorLabel.setText(errorText);
}

void ProgressDialog::start(const unsigned& maxVacancies)
{
    briefsProgressBar.setRange(0, maxVacancies);
    briefsProgressBar.setValue(0);

    allDetailsProgressBar.setRange(0, maxVacancies);
    allDetailsProgressBar.setValue(0);

    briefLabel.setText("Processing briefs...");
    detailsLabel.setText("Processing details...");

    errorLabel.setText("");

    exec();
}