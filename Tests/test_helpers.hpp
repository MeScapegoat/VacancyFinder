#pragma once

#include <gtest/gtest.h>

#include <Observable.hpp>

#include <concepts>

#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QString>
#include <QTest>
#include <QWidget>


template <typename T>
concept QWidgetDerived = std::derived_from<T, QWidget>;

template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;


template<QWidgetDerived Widget>
auto value(const Widget& view);

template<QWidgetDerived Widget>
void changeValue(Widget& view);


// QComboBox
template<>
auto value(const QComboBox& view)
{
    return view.currentIndex();
}

template<>
void changeValue(QComboBox& view)
{
    int newValue = (value(view) + 1) % view.count();
    view.setCurrentIndex(newValue);
}

// QSpinBox
template<>
auto value(const QSpinBox& view)
{
    return view.value();
}

template<>
void changeValue(QSpinBox& view)
{
    int newValue = value(view) + 1;
    view.setValue(newValue);
}

// QLineEdit
template<>
auto value(const QLineEdit& view)
{
    return view.text();
}

template<>
void changeValue(QLineEdit& view)
{
    const QString newValue = value(view) + "TEXT";
    QTest::keyClicks(&view, newValue);
}



template<class T>
void changeValue(Observable<T>& model);

// Numeric
template <Numeric T>
void changeValue(Observable<T>& model)
{
    model = model() + 1;
}


// QString
void changeValue(Observable<QString>& model) // TODO: maybe make general to take all kind of strings?
{
    model = model() + "TEXT";
}




template <class T, QWidgetDerived Widget>
void testModelAndViewSync(Observable<T>& model, Widget& view)
{
    EXPECT_EQ(model(), value(view)) << "Initial value of model and view differs";

    changeValue(model);
    EXPECT_EQ(model(), value(view));

    changeValue(view);
    EXPECT_EQ(model(), value(view));
}