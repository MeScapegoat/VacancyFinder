#include "VacancyBrowserView.h"

#include <QListView>
#include <QHBoxLayout>

#include "VacancyDetails.h"

VacancyBrowserView::VacancyBrowserView(QWidget* parent) :
    QWidget(parent),
    View(),
    m_vacancyList(*new QListView),
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

const QListView& VacancyBrowserView::vacancyList() const
{
    return m_vacancyList;
}

QListView& VacancyBrowserView::vacancyList()
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
