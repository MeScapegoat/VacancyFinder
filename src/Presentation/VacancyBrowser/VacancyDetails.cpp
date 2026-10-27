#include "VacancyDetails.h"

#include <QDate>
#include <QFrame>
#include <QFont>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QList>
#include <QPalette>
#include <QPushButton>
#include <QStackedLayout>
#include <QString>
#include <QTextBrowser>
#include <QUrl>
#include <QVBoxLayout>
#include <QWidget>

#include <Vacancy.h>

namespace
{
void setMetadataText(QLabel& label, const QString& heading, const QString& value)
{
    label.setText(QString("<span style='font-size:9pt;'>%1</span><br>"
                          "<span style='font-size:10pt;font-weight:600;'>%2</span>")
                      .arg(heading.toHtmlEscaped(), value.toHtmlEscaped()));
}

QString vacancyLink(const QUrl& url, const QString& text)
{
    return QString("<a href=\"%1\">%2</a>")
        .arg(url.toString(QUrl::FullyEncoded).toHtmlEscaped(), text.toHtmlEscaped());
}
}

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
    metadataPanel(*new QWidget(this)),
    statusButtonsPanel(*new QWidget(this)),
    statusButtonsLayout(*new QStackedLayout(&statusButtonsPanel)),
    ignoreButton(*new QPushButton),
    interestedButton(*new QPushButton),
    removeIgnoreButton(*new QPushButton),
    removeInterestedButton(*new QPushButton)
{
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);

    QVBoxLayout& mainLayout = *new QVBoxLayout;
    mainLayout.setContentsMargins(20, 18, 20, 18);
    mainLayout.setSpacing(10);

    titleLabel.setObjectName("vacancyTitle");
    titleLabel.setWordWrap(true);
    titleLabel.setOpenExternalLinks(true);
    QFont titleFont = titleLabel.font();
    titleFont.setPointSize(17);
    titleFont.setWeight(QFont::Bold);
    titleLabel.setFont(titleFont);
    mainLayout.addWidget(&titleLabel);

    companyNameLabel.setObjectName("vacancyCompany");
    companyNameLabel.setWordWrap(true);
    companyNameLabel.setOpenExternalLinks(true);
    QFont companyFont = companyNameLabel.font();
    companyFont.setPointSize(10);
    companyFont.setWeight(QFont::DemiBold);
    companyNameLabel.setFont(companyFont);
    mainLayout.addWidget(&companyNameLabel);

    QGridLayout& metadataLayout = *new QGridLayout(&metadataPanel);
    metadataLayout.setContentsMargins(0, 0, 0, 0);
    metadataLayout.setHorizontalSpacing(10);
    metadataLayout.setVerticalSpacing(8);

    const QList<QLabel*> metadataLabels{
        &employmentTypeLabel, &levelLabel, &experienceLabel, &educationLabel,
        &industryLabel, &expirationDateLabel, &skillsLabel
    };
    for (QLabel* label : metadataLabels)
    {
        label->setWordWrap(true);
        label->setTextFormat(Qt::RichText);
        label->setFrameShape(QFrame::StyledPanel);
        label->setBackgroundRole(QPalette::AlternateBase);
        label->setAutoFillBackground(true);
        label->setMargin(10);
    }

    metadataLayout.addWidget(&employmentTypeLabel, 0, 0);
    metadataLayout.addWidget(&levelLabel, 0, 1);
    metadataLayout.addWidget(&experienceLabel, 1, 0);
    metadataLayout.addWidget(&educationLabel, 1, 1);
    metadataLayout.addWidget(&industryLabel, 2, 0);
    metadataLayout.addWidget(&expirationDateLabel, 2, 1);
    metadataLayout.addWidget(&skillsLabel, 3, 0, 1, 2);
    metadataLayout.setColumnStretch(0, 1);
    metadataLayout.setColumnStretch(1, 1);
    mainLayout.addWidget(&metadataPanel);

    QLabel& descriptionHeading = *new QLabel("About this role", this);
    descriptionHeading.setObjectName("vacancyDescriptionHeading");
    QFont descriptionHeadingFont = descriptionHeading.font();
    descriptionHeadingFont.setPointSize(10);
    descriptionHeadingFont.setWeight(QFont::DemiBold);
    descriptionHeading.setFont(descriptionHeadingFont);
    mainLayout.addWidget(&descriptionHeading);

    vacancyDesc.setObjectName("vacancyDescription");
    vacancyDesc.setFrameShape(QFrame::StyledPanel);
    vacancyDesc.setOpenExternalLinks(true);
    vacancyDesc.setMinimumHeight(140);
    mainLayout.addWidget(&vacancyDesc, 1);

    ignoreButton.setText("Not interested");
    interestedButton.setText("Interested");
    removeIgnoreButton.setText("Remove from not interested list");
    removeInterestedButton.setText("Remove from interested list");

    for (QPushButton* button : {&ignoreButton, &interestedButton, &removeIgnoreButton, &removeInterestedButton})
    {
        button->setMinimumHeight(40);
        button->setCursor(Qt::PointingHandCursor);
    }

    QHBoxLayout& unreviewedLayout = *new QHBoxLayout;
    unreviewedLayout.setContentsMargins(0, 0, 0, 0);
    unreviewedLayout.setSpacing(10);
    unreviewedLayout.addWidget(&ignoreButton);
    unreviewedLayout.addWidget(&interestedButton);

    QWidget& unreviewedWidget = *new QWidget;
    unreviewedWidget.setLayout(&unreviewedLayout);

    statusButtonsLayout.insertWidget(static_cast<int>(VacancyStatus::Unreviewed), &unreviewedWidget);
    statusButtonsLayout.insertWidget(static_cast<int>(VacancyStatus::Ignored), &removeIgnoreButton);
    statusButtonsLayout.insertWidget(static_cast<int>(VacancyStatus::Interested), &removeInterestedButton);
    statusButtonsLayout.setContentsMargins(0, 6, 0, 0);
    mainLayout.addWidget(&statusButtonsPanel);
    statusButtonsPanel.hide();

    setLayout(&mainLayout);

    QObject::connect(&ignoreButton,     &QPushButton::released, this, &VacancyDetails::ignoreButtonPressed);
    QObject::connect(&interestedButton, &QPushButton::released, this, &VacancyDetails::interestedButtonPressed);

    QObject::connect(&removeIgnoreButton,     &QPushButton::released, this, &VacancyDetails::removeIgnoreButtonPressed);
    QObject::connect(&removeInterestedButton, &QPushButton::released, this, &VacancyDetails::removeInterestedButtonPressed);
}

void VacancyDetails::open(const Vacancy& vacancy, VacancyStatus status)
{
    updateCurrentStatusButton(status);
    metadataPanel.setVisible(true);
    statusButtonsPanel.setVisible(true);

    const QString& description = vacancy.description;
    const std::optional<QString>& level = vacancy.level;
    const std::optional<Months>& experience = vacancy.experience;
    const std::optional<QString>& industry = vacancy.industry;
    const std::optional<QString>& education = vacancy.education;
    const std::optional<QDate>& expirationDate = vacancy.expirationDate;
    const QString& skills = vacancy.skills;
    const QString& employmentType = vacancy.employmentType;

    titleLabel.setText(vacancyLink(vacancy.url,
                                   vacancy.title.isEmpty() ? QStringLiteral("Untitled vacancy") : vacancy.title));
    companyNameLabel.setText(vacancyLink(vacancy.companyUrl,
                                         vacancy.companyName.isEmpty() ? QStringLiteral("Company not provided") : vacancy.companyName));

    setMetadataText(employmentTypeLabel, QStringLiteral("Employment type"),
                    employmentType.isEmpty() ? QStringLiteral("Not provided") : employmentType);

    levelLabel.setVisible(level.has_value());
    if (level) setMetadataText(levelLabel, QStringLiteral("Level"), *level);

    experienceLabel.setVisible(experience.has_value());
    if (experience) setMetadataText(experienceLabel, QStringLiteral("Experience"), QStringLiteral("%1 years").arg(*experience / 12.0));

    educationLabel.setVisible(education.has_value());
    if (education) setMetadataText(educationLabel, QStringLiteral("Education"), *education);

    skillsLabel.setVisible(!skills.isEmpty());
    if (!skills.isEmpty()) setMetadataText(skillsLabel, QStringLiteral("Skills"), skills);

    industryLabel.setVisible(industry.has_value());
    if (industry) setMetadataText(industryLabel, QStringLiteral("Industry"), *industry);

    expirationDateLabel.setVisible(expirationDate.has_value());
    if (expirationDate) setMetadataText(expirationDateLabel, QStringLiteral("Expiration date"), expirationDate->toString("dd MMM yyyy"));

    vacancyDesc.setHtml(description);
}

void VacancyDetails::close()
{
    titleLabel.setText(QStringLiteral("Select a vacancy"));
    companyNameLabel.setText(QStringLiteral("Choose a role from the list to see its details."));
    metadataPanel.setVisible(false);
    statusButtonsPanel.setVisible(false);

    vacancyDesc.setHtml(R"(
        <!DOCTYPE html>
        <html lang="en">
        <body style="font-family: sans-serif;">
            <div style="text-align: center; margin-top: 36px;">
                <div style="font-size: 42px;">⌕</div>
                <h2 style="font-size: 18px;">No vacancy selected</h2>
                <p style="font-size: 13px;">Select a vacancy from the list to view its details.</p>
            </div>
        </body>
        </html>)");

    updateCurrentStatusButton(VacancyStatus::Unreviewed);
}

void VacancyDetails::updateCurrentStatusButton(VacancyStatus status)
{
    statusButtonsLayout.setCurrentIndex(static_cast<int>(status));
}
