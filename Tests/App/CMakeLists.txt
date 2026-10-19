add_executable(WorkCoordinator_test
	WorkCoordinator/WorkCoordinator_test.cpp
)

target_link_libraries(WorkCoordinator_test
	PRIVATE
		WorkCoordinator
		GTest::gtest_main
		Qt6::Test
)

set_target_properties(WorkCoordinator_test PROPERTIES FOLDER "Tests/App/WorkCoordinator")

gtest_discover_tests(WorkCoordinator_test)
