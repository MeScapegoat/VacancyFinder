#pragma once

#include <concepts>

#include <QObject>
#include <QWidget>

template<typename T>
concept QObjectType = std::derived_from<T, QObject>;

template<typename T>
concept QWidgetType = std::derived_from<T, QWidget>;

template<typename T>
concept ViewType = QWidgetType<T> && requires(T& value, const T& constValue)
{
    { value.widget() } -> std::same_as<QWidget&>;

    { constValue.widget() } -> std::same_as<const QWidget&>;
};

template<ViewType View, QObjectType Model>
class Presenter : public QObject
{
public:
    Presenter(QObject* parent = nullptr) :
        QObject(parent),
        view(*new View),
        model(*new Model)
    {
        model.setParent(this);
    }

    virtual ~Presenter() = default;

public:
    const View& getView() const
    {
        return view;
    };

    View& getView()
    {
        return view;
    }


    const Model& getModel() const
    {
        return model;
    }

    Model& getModel()
    {
        return model;
    };

private:
    View& view;
    Model& model;
};