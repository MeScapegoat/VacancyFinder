#include "VacancyDetails.h"

#include <QLabel>
#include <QTextBrowser>
#include <QStackedLayout>
#include <QPushButton>

#include <Vacancy.h>

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

    QObject::connect(&ignoreButton,     &QPushButton::released, this, &VacancyDetails::ignoreButtonPressed);
    QObject::connect(&interestedButton, &QPushButton::released, this, &VacancyDetails::interestedButtonPressed);

    QObject::connect(&removeIgnoreButton,     &QPushButton::released, this, &VacancyDetails::removeIgnoreButtonPressed);
    QObject::connect(&removeInterestedButton, &QPushButton::released, this, &VacancyDetails::removeInterestedButtonPressed);
}

void VacancyDetails::open(const Vacancy& vacancy, VacancyStatus status)
{
    updateCurrentStatusButton(status);

    const QString& title = vacancy.title;
    const QString& description = vacancy.description;
    const std::optional<QString>& level = vacancy.level;
    const std::optional<Months>& experience = vacancy.experience;
    const std::optional<QString>& industry = vacancy.industry;
    const std::optional<QString>& education = vacancy.education;
    const std::optional<QDate>& expirationDate = vacancy.expirationDate;
    const QString& skills = vacancy.skills;
    const QString& employmentType = vacancy.employmentType;
    const QString& companyName = vacancy.companyName;
    const QString& companyLink = vacancy.companyLink.toString();

    const QString linkPattern = "<a href='%1'>%2</a>";

    titleLabel.setText(linkPattern.arg(vacancy.link.toString()).arg(title));
    companyNameLabel.setText(linkPattern.arg(companyLink).arg(companyName));

    employmentTypeLabel.setText("Employment type: " + employmentType);


    levelLabel.setVisible(level.has_value());
    if (level) levelLabel.setText("Level: " + *level);

    experienceLabel.setVisible(experience.has_value());
    if (experience) experienceLabel.setText("Experience: " + QString::number(*experience / 12.0) + " years");

    educationLabel.setVisible(education.has_value());
    if (education) educationLabel.setText("Education: " + *education);

    skillsLabel.setVisible(!skills.isEmpty());
    if (!skills.isEmpty()) skillsLabel.setText("Skills: " + skills);

    industryLabel.setVisible(industry.has_value());
    if (industry) industryLabel.setText("Industry: " + *industry);

    expirationDateLabel.setVisible(expirationDate.has_value());
    if (expirationDate) expirationDateLabel.setText("Expiration date: " + expirationDate->toString("dd-MM-yyyy"));

    vacancyDesc.setHtml(description);
}

void VacancyDetails::close()
{
    titleLabel.setText("Title");
    companyNameLabel.setText("Company");

    employmentTypeLabel.setText("Employment type");

    levelLabel.setVisible(true);
    levelLabel.setText("Level");

    experienceLabel.setVisible(true);
    experienceLabel.setText("Experience");

    educationLabel.setVisible(true);
    educationLabel.setText("Education");

    skillsLabel.setVisible(true);
    skillsLabel.setText("Skills");

    industryLabel.setVisible(true);
    industryLabel.setText("Industry");

    expirationDateLabel.setVisible(true);
    expirationDateLabel.setText("Expiration date");


    vacancyDesc.setHtml(R"(
        <!DOCTYPE html>
        <html lang="en">
        <body>
            <header>
                <h1>Choose vacancy from left list to see its description here</h1>
            </header>
        </body>
        </html>)");

    updateCurrentStatusButton(VacancyStatus::Unreviewed);
}

void VacancyDetails::updateCurrentStatusButton(VacancyStatus status)
{
    statusButtonsLayout.setCurrentIndex(static_cast<int>(status));
}
