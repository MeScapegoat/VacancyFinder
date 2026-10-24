#pragma once

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QNetworkReply>

class Vacancy;


class ProgressDialog : public QDialog
{
public:
    ProgressDialog(QWidget* parent);
    ~ProgressDialog() = default;

public slots:
    void updateVacancyCount(int count);
    void updateBrief(const Vacancy& vacancy);
    void updateDetails(const Vacancy& vacancy);
    void updateError(QNetworkReply::NetworkError);
    void start(qsizetype maxVacancies);

public:
    QPushButton& stopButton;

private:
    QLabel& briefLabel;
    QProgressBar& briefsProgressBar;


    QLabel& detailsLabel;
    QProgressBar& allDetailsProgressBar;

    QLabel& errorLabel;
};