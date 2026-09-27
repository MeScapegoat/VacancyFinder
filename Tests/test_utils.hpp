#pragma once

#include <gtest/gtest.h>

#include <concepts>

#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QString>
#include <QTest>

template <typename T>
concept QObjectDerived = std::derived_from<T, QObject>;

template <typename T>
concept QWidgetDerived = std::derived_from<T, QWidget>;

template <typename T>
concept Numeric = (std::integral<T> || std::floating_point<T>) && !std::same_as<T, bool>;

template <typename T>
concept HasCount = requires(const T & value)
{
    { value.count() } -> Numeric;
};



// Numeric
template <Numeric T>
T otherValue(T value)
{
    return value + 1;
}

// QString
QString otherValue(QString value) // TODO: maybe make more general to accept all kind of string?
{
    return value + "TEXT";
}

// bool
bool otherValue(bool value)
{
    return !value;
}



// QComboBox
auto value(const QComboBox& view)
{
    return view.currentIndex();
}

void changeValue(QComboBox& view)
{
    const int newValue = otherValue(value(view)) % view.count();
    view.setCurrentIndex(newValue);
}


// QSpinBox
auto value(const QSpinBox& view)
{
    return view.value();
}

void changeValue(QSpinBox& view)
{
    const int newValue = otherValue(value(view));
    view.setValue(newValue);
}


// QLineEdit
auto value(const QLineEdit& view)
{
    return view.text();
}

void changeValue(QLineEdit& view)
{
    const QString newValue = otherValue(value(view));
    QTest::keyClicks(&view, newValue);
}


// QCheckBox
auto value(const QCheckBox& view)
{
    return view.isChecked();
}

void changeValue(QCheckBox& view)
{
    const bool newValue = otherValue(value(view));
    view.setChecked(newValue);
}