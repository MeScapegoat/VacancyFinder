#pragma once

#include <QWidget>

class QLabel;
class QTextBrowser;
class QStackedLayout;
class QPushButton;

class VacancyDetails : public QWidget
{
public:
    explicit VacancyDetails(QWidget* parent = nullptr);
    ~VacancyDetails() = default;

public:
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