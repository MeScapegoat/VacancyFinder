#include "VacancyBrowserView.h"

#include <QFont>
#include <QHBoxLayout>
#include <QLabel>
#include <QPalette>
#include <QVBoxLayout>
#include <QWidget>

#include "VacancyDetails.h"

VacancyBrowserView::VacancyBrowserView(QWidget* parent) :
    View(parent),
    m_vacancyList(*new VacancyList(this)),
    m_vacancyDetails(*new VacancyDetails(this))
{
    QWidget& listPanel = *new QWidget(this);
    listPanel.setObjectName("vacancyListPanel");

    QVBoxLayout& listPanelLayout = *new QVBoxLayout(&listPanel);
    listPanelLayout.setContentsMargins(0, 0, 0, 0);
    listPanelLayout.setSpacing(8);

    QLabel& heading = *new QLabel("Your vacancies", &listPanel);
    heading.setObjectName("vacancyListHeading");
    QFont headingFont = heading.font();
    headingFont.setPointSize(12);
    headingFont.setWeight(QFont::DemiBold);
    heading.setFont(headingFont);
    listPanelLayout.addWidget(&heading);

    QLabel& caption = *new QLabel("Select a role to review its details", &listPanel);
    caption.setObjectName("vacancyListCaption");
    QFont captionFont = caption.font();
    captionFont.setPointSize(8);
    caption.setFont(captionFont);
    caption.setForegroundRole(QPalette::WindowText);
    listPanelLayout.addWidget(&caption);
    listPanelLayout.addWidget(&m_vacancyList, 1);

    QHBoxLayout& mainLayout = *new QHBoxLayout;
    mainLayout.setContentsMargins(22, 18, 22, 22);
    mainLayout.setSpacing(18);
    mainLayout.addWidget(&listPanel, 2);
    mainLayout.addWidget(&m_vacancyDetails);
    mainLayout.setStretch(1, 3);

    setLayout(&mainLayout);
    setObjectName("vacancyBrowserView");
    setBackgroundRole(QPalette::Window);
    setAutoFillBackground(true);
}

const VacancyList& VacancyBrowserView::vacancyList() const
{
    return m_vacancyList;
}

VacancyList& VacancyBrowserView::vacancyList()
{
    return m_vacancyList;
}

const VacancyDetails& VacancyBrowserView::vacancyDetails() const
{
    return m_vacancyDetails;
}

VacancyDetails& VacancyBrowserView::vacancyDetails()
{
    return m_vacancyDetails;
}
