#pragma once

#include <QDialog>
#include <QLabel>
#include <QNetworkReply>
#include <QProgressBar>
#include <QPushButton>

class Vacancy;


class ProgressDialog : public QDialog
{
public:
    ProgressDialog(QWidget* parent);
    ~ProgressDialog() = default;

public slots:
    void updateVacancyCount(int count);
    void updateSummary(const Vacancy& vacancy);
    void updateDetails(const Vacancy& vacancy);
    void start(qsizetype maxVacancies);

public:
    const QPushButton& stopButton() const;
    QPushButton& stopButton();

private:
    QPushButton& m_stopButton;
    QLabel& summaryLabel;
    QProgressBar& summariesProgressBar;
    QLabel& detailsLabel;
    QProgressBar& allDetailsProgressBar;
    QLabel& errorLabel;
};
