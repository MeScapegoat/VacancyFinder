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

gtest_discover_tests(ConnectionGroup_test)