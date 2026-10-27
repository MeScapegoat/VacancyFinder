#pragma once

#include <QListView>
#include <QWidget>

class VacancyList final : public QListView
{
public:
    explicit VacancyList(QWidget* parent = nullptr);
};
