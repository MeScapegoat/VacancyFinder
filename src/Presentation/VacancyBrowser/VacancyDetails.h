#pragma once

#include <QWidget>

#include <VacancyStatus.h>

class QLabel;
class QTextBrowser;
class QStackedLayout;
class QPushButton;
class Vacancy;

class VacancyDetails : public QWidget
{
    Q_OBJECT

public:
    explicit VacancyDetails(QWidget* parent = nullptr);
    ~VacancyDetails() = default;

public:
    void open(const Vacancy& vacancy, VacancyStatus status, bool isCompanyIgnored, bool canIgnoreCompany);
    void close();

    void updateCurrentStatusButton(VacancyStatus status);
    void updateCompanyIgnoreButton(bool isIgnored, bool canIgnoreCompany);

signals:
    void ignoreButtonPressed();
    void interestedButtonPressed();

    void removeIgnoreButtonPressed();
    void removeInterestedButtonPressed();
    void companyIgnoreButtonPressed();

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
    QWidget& metadataPanel;

    QWidget& statusButtonsPanel;
    QStackedLayout& statusButtonsLayout;

    QPushButton& ignoreButton;
    QPushButton& interestedButton;

    QPushButton& removeIgnoreButton;
    QPushButton& removeInterestedButton;
    QPushButton& companyIgnoreButton;
};
