#pragma once

#include <QWidget>

class QAbstractItemModel;
class QLabel;
class QListView;

class VacancyList final : public QWidget
{
public:
    explicit VacancyList(QWidget* parent = nullptr);

    QListView& listView();
    const QListView& listView() const;

    void setModel(QAbstractItemModel* model);

private:
    void updateVisibleVacancyCount();

private:
    QListView& m_listView;
    QLabel& m_visibleVacancyCount;
};
