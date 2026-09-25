#pragma once

class QWidget;

class View
{
public:
    View() = default;
    virtual ~View() = default;

public:
    virtual QWidget& widget() = 0;
    virtual const QWidget& widget() const = 0;
};