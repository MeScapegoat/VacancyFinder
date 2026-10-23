add_executable(ConnectionGroup_test
	ConnectionGroup_test.cpp
)

target_link_libraries(ConnectionGroup_test
	PRIVATE
		ConnectionGroup
		GTest::gtest_main
		Qt6::Core
)

set_target_properties(ConnectionGroup_test PROPERTIES FOLDER "Tests/Utils")

gtest_discover_tests(ConnectionGroup_test PROPERTIES LABELS "unit;Utils")



add_executable(Utils_test
	Utils_test.cpp
)

target_link_libraries(Utils_test
	PRIVATE
		Utils
		GTest::gtest_main
		Qt6::Test
)

set_target_properties(Utils_test PROPERTIES FOLDER "Tests/Utils")

gtest_discover_tests(Utils_test PROPERTIES LABELS "unit;Utils")



add_executable(CallableComboBox_test
	CallableComboBox_test.cpp
)

target_link_libraries(CallableComboBox_test
	PRIVATE
		main_for_qt_tests
		CallableComboBox
		Qt6::Widgets
)

set_target_properties(CallableComboBox_test PROPERTIES FOLDER "Tests/Utils")

gtest_discover_tests(CallableComboBox_test PROPERTIES LABELS "unit;Utils")
