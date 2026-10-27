#pragma once

#include <QWidget>

#include <VacancyList.h>
#include <View.h>

class VacancyDetails;

class VacancyBrowserView : public View
{
public:
    explicit VacancyBrowserView(QWidget* parent = nullptr);
    ~VacancyBrowserView() = default;

public:
    const VacancyList& vacancyList() const;
    VacancyList& vacancyList();

    const VacancyDetails& vacancyDetails() const;
    VacancyDetails& vacancyDetails();

private:
    VacancyList& m_vacancyList;
    VacancyDetails& m_vacancyDetails;
};
