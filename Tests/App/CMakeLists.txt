add_executable(AppController_test
	AppController/AppController_test.cpp
)

target_link_libraries(AppController_test
	PRIVATE
		App
		GTest::gtest_main
		Qt6::Test
)

set_target_properties(AppController_test PROPERTIES FOLDER "Tests/App/AppController")

gtest_discover_tests(AppController_test)