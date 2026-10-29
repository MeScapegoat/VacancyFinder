#include "ProgressDialog.h"

#include <QFont>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QPalette>
#include <QProgressBar>
#include <QPushButton>
#include <QString>
#include <QTime>
#include <QVBoxLayout>
#include <QWidget>

#include <Vacancy.h>

ProgressDialog::ProgressDialog(QWidget* parent) :
    QDialog(parent),
    m_stopButton(*new QPushButton),
    summaryLabel(*new QLabel),
    summariesProgressBar(*new QProgressBar),
    detailsLabel(*new QLabel),
    allDetailsProgressBar(*new QProgressBar),
    errorLabel(*new QLabel)
{
    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint);
    setWindowTitle("Search progress");
    setObjectName("progressDialog");
    setBackgroundRole(QPalette::Window);
    setAutoFillBackground(true);

    QVBoxLayout& dialogLayout = *new QVBoxLayout(this);
    dialogLayout.setContentsMargins(0, 0, 0, 0);
    dialogLayout.setSpacing(0);

    QFrame& dialogFrame = *new QFrame(this);
    dialogFrame.setObjectName("progressDialogFrame");
    dialogFrame.setFrameShape(QFrame::Box);
    dialogFrame.setFrameShadow(QFrame::Plain);
    dialogFrame.setLineWidth(2);
    dialogFrame.setForegroundRole(QPalette::Light);
    dialogFrame.setBackgroundRole(QPalette::Window);
    dialogFrame.setAutoFillBackground(true);

    QVBoxLayout& layout = *new QVBoxLayout(&dialogFrame);
    layout.setContentsMargins(22, 20, 22, 20);
    layout.setSpacing(12);

    QLabel& title = *new QLabel("Search progress", &dialogFrame);
    QFont titleFont = title.font();
    titleFont.setPointSize(16);
    titleFont.setWeight(QFont::DemiBold);
    title.setFont(titleFont);
    layout.addWidget(&title);

    QFrame& summariesCard = *new QFrame(&dialogFrame);
    summariesCard.setFrameShape(QFrame::StyledPanel);
    summariesCard.setBackgroundRole(QPalette::AlternateBase);
    summariesCard.setAutoFillBackground(true);

    QVBoxLayout& summariesLayout = *new QVBoxLayout(&summariesCard);
    summariesLayout.setContentsMargins(14, 12, 14, 12);
    summariesLayout.setSpacing(7);

    QLabel& summariesHeading = *new QLabel("Vacancy summaries", &summariesCard);
    QFont sectionFont = summariesHeading.font();
    sectionFont.setWeight(QFont::DemiBold);
    summariesHeading.setFont(sectionFont);
    summariesLayout.addWidget(&summariesHeading);

    summaryLabel.setTextFormat(Qt::PlainText);
    summaryLabel.setWordWrap(true);
    summariesLayout.addWidget(&summaryLabel);

    summariesProgressBar.setTextVisible(true);
    summariesProgressBar.setFormat("%v / %m");
    summariesProgressBar.setMinimumHeight(20);
    summariesLayout.addWidget(&summariesProgressBar);

    layout.addWidget(&summariesCard, 1);

    QFrame& detailsCard = *new QFrame(&dialogFrame);
    detailsCard.setFrameShape(QFrame::StyledPanel);
    detailsCard.setBackgroundRole(QPalette::AlternateBase);
    detailsCard.setAutoFillBackground(true);

    QVBoxLayout& detailsLayout = *new QVBoxLayout(&detailsCard);
    detailsLayout.setContentsMargins(14, 12, 14, 12);
    detailsLayout.setSpacing(7);

    QLabel& detailsHeading = *new QLabel("Vacancy details", &detailsCard);
    detailsHeading.setFont(sectionFont);
    detailsLayout.addWidget(&detailsHeading);

    detailsLabel.setTextFormat(Qt::PlainText);
    detailsLabel.setWordWrap(true);
    detailsLayout.addWidget(&detailsLabel);

    allDetailsProgressBar.setTextVisible(true);
    allDetailsProgressBar.setFormat("%v / %m");
    allDetailsProgressBar.setMinimumHeight(20);
    detailsLayout.addWidget(&allDetailsProgressBar);

    layout.addWidget(&detailsCard, 1);

    errorLabel.setTextFormat(Qt::PlainText);
    errorLabel.setWordWrap(true);
    errorLabel.setForegroundRole(QPalette::WindowText);
    QFont errorFont = errorLabel.font();
    errorFont.setWeight(QFont::DemiBold);
    errorLabel.setFont(errorFont);
    errorLabel.hide();
    layout.addWidget(&errorLabel);

    m_stopButton.setText("Stop search");
    m_stopButton.setMinimumSize(130, 38);

    QHBoxLayout& buttonLayout = *new QHBoxLayout;
    buttonLayout.addStretch(1);
    buttonLayout.addWidget(&m_stopButton);
    layout.addLayout(&buttonLayout);
    dialogLayout.addWidget(&dialogFrame);

    setMinimumSize(560, 380);
    resize(620, 420);

    QObject::connect(&m_stopButton, &QPushButton::released, this, &ProgressDialog::accept);
}

const QPushButton& ProgressDialog::stopButton() const
{
    return m_stopButton;
}

QPushButton& ProgressDialog::stopButton()
{
    return m_stopButton;
}

void ProgressDialog::updateVacancyCount(int count)
{
    summariesProgressBar.setMaximum(count);
    allDetailsProgressBar.setMaximum(count);
}

void ProgressDialog::updateSummary(const Vacancy& vacancy)
{
    static constexpr const char* summaryPattern = "Processed summary #%1 — %2 at %3";

    summaryLabel.setText(QString(summaryPattern)
                       .arg(vacancy.id)
                       .arg(vacancy.title)
                       .arg(vacancy.companyName));

    summariesProgressBar.setValue(vacancy.id);
}

void ProgressDialog::updateDetails(const Vacancy& vacancy)
{
    static constexpr const char* detailsPattern = "Processed details for vacancy #%1";

    detailsLabel.setText(QString(detailsPattern).arg(vacancy.id));

    allDetailsProgressBar.setValue(vacancy.id);
}

void ProgressDialog::start(qsizetype maxVacancies)
{
    summariesProgressBar.setRange(0, maxVacancies);
    summariesProgressBar.setValue(0);

    allDetailsProgressBar.setRange(0, maxVacancies);
    allDetailsProgressBar.setValue(0);

    summaryLabel.setText("Processing summaries...");
    detailsLabel.setText("Processing vacancy details...");

    errorLabel.setText("");
    errorLabel.hide();

    exec();
}
