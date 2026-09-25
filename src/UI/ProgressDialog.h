#pragma once

// std

// Qt
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QNetworkReply>

// custom
//#include "../model/Vacancy.h"

class Vacancy;


class ProgressDialog : public QDialog
{
public:
    ProgressDialog(QWidget* parent);
    ~ProgressDialog() = default;

public slots:
    void updateBrief(const Vacancy& vacancy);
    void updateDetails(const Vacancy& vacancy);
    void updateError(QNetworkReply::NetworkError);
    void start(const unsigned& maxVacancies);

public:
    QPushButton& stopButton;

private:
    QLabel& briefLabel;
    QProgressBar& briefsProgressBar;


    QLabel& detailsLabel;
    QProgressBar& allDetailsProgressBar;

    QLabel& errorLabel;
};