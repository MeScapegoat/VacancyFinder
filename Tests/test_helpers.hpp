#pragma once
#include "test_utils.hpp"

#include <Observable.hpp>

#include <QPushButton>
#include <QSignalSpy>


template <class T, QWidgetDerived Widget, std::integral N>
requires HasCount<Widget>
void testModelAndViewSync(Observable<T>& model, Widget& view, N modesAmount)
{
    EXPECT_EQ(view.count(), modesAmount) << "Not all available modes are reflected in UI";

    testModelAndViewSync(model, view);
}

template <class T, QWidgetDerived Widget>
void testModelAndViewSync(Observable<T>& model, Widget& view)
{
    EXPECT_EQ(model(), value(view)) << "Initial value of model and view differs";

    model = otherValue(model());
    EXPECT_EQ(model(), value(view));

    changeValue(view);
    EXPECT_EQ(model(), value(view));
}


template <QObjectDerived SignalOwner, typename Signal>
void testButtonSignalEmitted(SignalOwner& signalOwner, Signal signal, QPushButton& button)
{
    QSignalSpy spy(&signalOwner, signal);
    QTest::mouseClick(&button, Qt::LeftButton);
    EXPECT_EQ(spy.count(), 1);
}