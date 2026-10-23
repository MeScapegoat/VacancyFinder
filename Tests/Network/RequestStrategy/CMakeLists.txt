add_executable(LinkedInRequestStrategy_test
	LinkedInRequestStrategy_test.cpp
)

target_link_libraries(LinkedInRequestStrategy_test
	PRIVATE
		main_for_qt_tests
		LinkedInRequestStrategy
		FakeNetworkManager
		FakeNetworkReply
		GTest::gtest
		Qt6::Test
)

set_target_properties(LinkedInRequestStrategy_test PROPERTIES FOLDER "Tests/Network/RequestStrategy")

gtest_discover_tests(LinkedInRequestStrategy_test PROPERTIES LABELS "unit;Network")
