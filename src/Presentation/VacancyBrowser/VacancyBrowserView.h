#pragma once

#include <View.h>
#include <QWidget>

class QListView;
class VacancyDetails;

class VacancyBrowserView : public QWidget, public View
{
public:
    explicit VacancyBrowserView(QWidget* parent = nullptr);
    ~VacancyBrowserView() = default;

public:
    // View
    QWidget& widget() override;
    const QWidget& widget() const override;

public:
    const QListView& vacancyList() const;
    QListView& vacancyList();

    const VacancyDetails& vacancyDetails() const;
    VacancyDetails& vacancyDetails();

private:
    QListView& m_vacancyList;
    VacancyDetails& m_vacancyDetails;
};