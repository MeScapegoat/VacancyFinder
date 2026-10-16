#pragma once

#include <View.h>
#include <QWidget>

class QListView;
class VacancyDetails;

class VacancyBrowserView : public QWidget, public View
{
public:
    using VacancyList = QListView; // TODO: make pretty

public:
    explicit VacancyBrowserView(QWidget* parent = nullptr);
    ~VacancyBrowserView() = default;

public:
    // View
    QWidget& widget() override;
    const QWidget& widget() const override;

public:
    const VacancyList& vacancyList() const;
    VacancyList& vacancyList();

    const VacancyDetails& vacancyDetails() const;
    VacancyDetails& vacancyDetails();

private:
    VacancyList& m_vacancyList;
    VacancyDetails& m_vacancyDetails;
};