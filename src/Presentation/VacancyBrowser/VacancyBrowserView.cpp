#include "VacancyBrowserView.h"

#include <QListView>
#include <QHBoxLayout>

#include "VacancyDetails.h"

VacancyBrowserView::VacancyBrowserView(QWidget* parent) :
    QWidget(parent),
    View(),
    m_vacancyList(*new VacancyList),
    m_vacancyDetails(*new VacancyDetails)
{
    QHBoxLayout& mainLayout = *new QHBoxLayout;

    mainLayout.addWidget(&m_vacancyList);
    mainLayout.addWidget(&m_vacancyDetails);

    setLayout(&mainLayout);
}

QWidget& VacancyBrowserView::widget()
{
    return *this;
}

const QWidget& VacancyBrowserView::widget() const
{
    return *this;
}

const VacancyBrowserView::VacancyList& VacancyBrowserView::vacancyList() const
{
    return m_vacancyList;
}

VacancyBrowserView::VacancyList& VacancyBrowserView::vacancyList()
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
