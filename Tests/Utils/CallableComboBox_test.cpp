#include <gtest/gtest.h>

#include <functional>

#include <QIcon>
#include <QString>

#include "CallableComboBox.hpp"

TEST(CallableComboBoxTest, KeepsFunctionsAlignedWhenInsertingItems)
{
    using ItemFunction = std::function<int()>;
    CallableComboBox<ItemFunction> comboBox;

    comboBox.insertItem(0, "First", []() { return 1; });
    comboBox.insertItem(1, QIcon(), "Last", []() { return 3; }, QString("payload"));
    comboBox.insertItem(1, "Middle", []() { return 2; });

    ASSERT_EQ(comboBox.count(), 3);
    EXPECT_EQ(comboBox.itemText(2), QString("Last"));
    EXPECT_EQ(comboBox.itemData(2).toString(), QString("payload"));

    comboBox.setCurrentIndex(0);
    EXPECT_EQ(comboBox.currentFunction()(), 1);

    comboBox.setCurrentIndex(1);
    EXPECT_EQ(comboBox.currentFunction()(), 2);

    comboBox.setCurrentIndex(2);
    EXPECT_EQ(comboBox.currentFunction()(), 3);
}
