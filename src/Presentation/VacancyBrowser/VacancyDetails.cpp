#include "VacancyDetails.h"

#include <QLabel>
#include <QTextBrowser>
#include <QStackedLayout>
#include <QPushButton>

#include "VacancyBrowserEnums.h"

VacancyDetails::VacancyDetails(QWidget* parent) :
    QWidget(parent),
    titleLabel(*new QLabel),
    companyNameLabel(*new QLabel),
    employmentTypeLabel(*new QLabel),
    levelLabel(*new QLabel),
    experienceLabel(*new QLabel),
    educationLabel(*new QLabel),
    skillsLabel(*new QLabel),
    industryLabel(*new QLabel),
    expirationDateLabel(*new QLabel),
    vacancyDesc(*new QTextBrowser),
    statusButtonsLayout(*new QStackedLayout),
    ignoreButton(*new QPushButton),
    interestedButton(*new QPushButton),
    removeIgnoreButton(*new QPushButton),
    removeInterestedButton(*new QPushButton)
{
    QVBoxLayout& mainLayout = *new QVBoxLayout;

    //titleLabel.setStyleSheet("QLabel { font-size: 22px; }");
    titleLabel.setOpenExternalLinks(true);
    mainLayout.addWidget(&titleLabel);

    //companyNameLabel.setStyleSheet("QLabel { font-size: 22px; }");
    companyNameLabel.setOpenExternalLinks(true);
    mainLayout.addWidget(&companyNameLabel);

    //employmentTypeLabel.setStyleSheet("QLabel { font-size: 18px; }");
    mainLayout.addWidget(&employmentTypeLabel);

    //levelLabel.setStyleSheet("QLabel { font-size: 18px; }");
    mainLayout.addWidget(&levelLabel);

    //experienceLabel.setStyleSheet("QLabel { font-size: 18px; }");
    mainLayout.addWidget(&experienceLabel);

    //educationLabel.setStyleSheet("QLabel { font-size: 18px; }");
    mainLayout.addWidget(&educationLabel);

    //skillsLabel.setStyleSheet("QLabel { font-size: 18px; }");
    mainLayout.addWidget(&skillsLabel);

    //industryLabel.setStyleSheet("QLabel { font-size: 18px; }");
    mainLayout.addWidget(&industryLabel);

    //expirationDateLabel.setStyleSheet("QLabel { font-size: 18px; }");
    mainLayout.addWidget(&expirationDateLabel);


    //vacancyDesc.setStyleSheet("QTextBrowser { font-size: 16px; }");
    mainLayout.addWidget(&vacancyDesc);


    ignoreButton.setText("Not interested");
    interestedButton.setText("Interested");

    removeIgnoreButton.setText("Remove from not interested list");
    removeInterestedButton.setText("Remove from interested list");

    QHBoxLayout& unreviewedLayout = *new QHBoxLayout;
    unreviewedLayout.addWidget(&ignoreButton);
    unreviewedLayout.addWidget(&interestedButton);

    QWidget& unreviewedWidget = *new QWidget;
    unreviewedWidget.setLayout(&unreviewedLayout);

    statusButtonsLayout.insertWidget(static_cast<int>(VacancyStatus::Unreviewed), &unreviewedWidget);
    statusButtonsLayout.insertWidget(static_cast<int>(VacancyStatus::Ignored), &removeIgnoreButton);
    statusButtonsLayout.insertWidget(static_cast<int>(VacancyStatus::Interested), &removeInterestedButton);

    mainLayout.addLayout(&statusButtonsLayout);

    setLayout(&mainLayout);
}