#include "VacancyBrowserView.h"

#include <QHBoxLayout>
#include <QPalette>
#include <QWidget>

#include "VacancyDetails.h"

VacancyBrowserView::VacancyBrowserView(QWidget* parent) :
    View(parent),
    m_vacancyList(*new VacancyList(this)),
    m_vacancyDetails(*new VacancyDetails(this))
{
    QHBoxLayout& mainLayout = *new QHBoxLayout;
    mainLayout.setContentsMargins(22, 18, 22, 22);
    mainLayout.setSpacing(18);
    mainLayout.addWidget(&m_vacancyList, 2);
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
