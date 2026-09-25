#pragma once

#include <QObject>

// Qt moc doesnt support templates, so Q_OBJECT macro in separate non-template class
class AbstractObservable : public QObject
{
    Q_OBJECT

public:
    AbstractObservable() = default;
    virtual ~AbstractObservable() = default;

signals:
    void valueChanged();
};


template<std::equality_comparable T>
class Observable : public AbstractObservable
{
public:
    Observable() = default;

    Observable(T value_) : 
    value(std::move(value_))
    {}

    ~Observable() = default;

public:
    Observable& operator=(const Observable& other)
    {
        setValue(other());
        return *this;
    }

    Observable& operator=(const T& other)
    {
        setValue(other);
        return *this;
    }

public:
    const T& operator()() const
    {
        return getValue();
    }

    const T& getValue() const
    {
        return value;
    }


public:
    void setValue(const T& newValue)
    {
        if (value == newValue)
            return;

        value = newValue;
        emit valueChanged();
    }

private:
    T value;
};