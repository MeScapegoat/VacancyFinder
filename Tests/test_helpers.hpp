#pragma once
#include "test_utils.hpp"

#include <Observable.hpp>

#include <QPushButton>
#include <QSignalSpy>


template <class T, QWidgetDerived Widget, std::integral N>
requires HasCount<Widget>
void testModelAndViewSync(Observable<T>& model, Widget& view, N modesAmount)
{
    EXPECT_EQ(view.count(), modesAmount) << "UI does not contain all available modes";

    testModelAndViewSync(model, view);
}

template <class T, QWidgetDerived Widget>
void testModelAndViewSync(Observable<T>& model, Widget& view)
{
    EXPECT_EQ(model(), value(view)) << "Initial model and view values differ";

    model = otherValue(model());
    EXPECT_EQ(model(), value(view)) << "Model and view values differ after changing the model";

    changeValue(view);
    EXPECT_EQ(model(), value(view)) << "Model and view values differ after changing the view";
}


template <QObjectDerived SignalOwner, typename Signal>
void testButtonSignalEmitted(SignalOwner& signalOwner, Signal signal, QPushButton& button)
{
    QSignalSpy spy(&signalOwner, signal);
    QTest::mouseClick(&button, Qt::LeftButton);
    EXPECT_EQ(spy.count(), 1);
}