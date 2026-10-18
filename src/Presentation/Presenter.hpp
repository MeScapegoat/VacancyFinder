#pragma once

#include <concepts>

#include <QObject>
#include <QWidget>

template<typename T>
concept QObjectType = std::derived_from<T, QObject>;

template<typename T>
concept QWidgetType = std::derived_from<T, QWidget>;


template<QWidgetType View, QObjectType Model>
class Presenter : public QObject
{
public:
    Presenter(QObject* parent = nullptr) :
        QObject(parent),
        m_view(*new View),
        m_model(*new Model)
    {
        m_model.setParent(this);
    }

    virtual ~Presenter() = default;

public:
    QWidget& viewWidget()
    {
        return m_view;
    }

    const QWidget& viewWidget() const
    {
        return m_view;
    }

protected:
    const View& view() const
    {
        return m_view;
    };

    View& view()
    {
        return m_view;
    }


    const Model& model() const
    {
        return m_model;
    }

    Model& model()
    {
        return m_model;
    };

private:
    View& m_view;
    Model& m_model;
};