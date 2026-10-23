add_executable(RequestManager_test
	RequestManager_test.cpp
)

target_link_libraries(RequestManager_test
	PRIVATE
		RequestManager
		AbstractRequestStrategy
		FakeNetworkManager
		main_for_qt_tests
		GTest::gtest
		Qt6::Core
		Qt6::Test
)

set_target_properties(RequestManager_test PROPERTIES FOLDER "Tests/Network")

gtest_discover_tests(RequestManager_test PROPERTIES LABELS "unit;Network")



add_executable(FakeNetworkReply_test
	FakeNetworkReply_test.cpp
)

target_link_libraries(FakeNetworkReply_test
	PRIVATE
		FakeNetworkReply
		GTest::gtest_main
		Qt6::Test
)

set_target_properties(FakeNetworkReply_test PROPERTIES FOLDER "Tests/Network")

gtest_discover_tests(FakeNetworkReply_test PROPERTIES LABELS "unit;Network")



add_executable(FakeNetworkManager_test
	FakeNetworkManager_test.cpp
)

target_link_libraries(FakeNetworkManager_test
	PRIVATE
		FakeNetworkManager
		FakeNetworkReply
		GTest::gtest_main
		Qt6::Test
)

set_target_properties(FakeNetworkManager_test PROPERTIES FOLDER "Tests/Network")

gtest_discover_tests(FakeNetworkManager_test PROPERTIES LABELS "unit;Network")



add_subdirectory(RequestStrategy)
add_subdirectory(SearchUrlProvider)
