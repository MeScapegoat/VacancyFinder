#pragma once

#include <QWidget>

class View : public QWidget
{
public:
    View(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    };

    virtual ~View() = default;
};