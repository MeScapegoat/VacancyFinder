#pragma once

#include <QWidget>

#include <VacancyStatus.h>

class QLabel;
class QTextBrowser;
class QStackedLayout;
class QPushButton;
class Vacancy;

// TODO: make pretty
class VacancyDetails : public QWidget
{
    Q_OBJECT

public:
    explicit VacancyDetails(QWidget* parent = nullptr);
    ~VacancyDetails() = default;

public:
    void open(const Vacancy& vacancy, VacancyStatus status);
    void close();

    void updateCurrentStatusButton(VacancyStatus status);

signals:
    void ignoreButtonPressed();
    void interestedButtonPressed();

    void removeIgnoreButtonPressed();
    void removeInterestedButtonPressed();

private:
    QLabel& titleLabel;
    QLabel& companyNameLabel;
    QLabel& employmentTypeLabel;
    QLabel& levelLabel;
    QLabel& experienceLabel;
    QLabel& educationLabel;
    QLabel& skillsLabel;
    QLabel& industryLabel;
    QLabel& expirationDateLabel;
    QTextBrowser& vacancyDesc;


    QStackedLayout& statusButtonsLayout;

    QPushButton& ignoreButton;
    QPushButton& interestedButton;

    QPushButton& removeIgnoreButton;
    QPushButton& removeInterestedButton;
};