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

gtest_discover_tests(WorkCoordinator_test PROPERTIES LABELS "unit;App")

add_executable(VacancySearchWorkflow_test
	VacancySearchWorkflow/VacancySearchWorkflow_test.cpp
)

target_link_libraries(VacancySearchWorkflow_test
	PRIVATE
		VacancySearchWorkflow
		AbstractSearchUrlProvider
		AbstractParserStrategy
		AbstractRequestStrategy
		FakeNetworkManager
		Parser
		RequestManager
		main_for_qt_tests
		GTest::gtest
		Qt6::Test
)

set_target_properties(VacancySearchWorkflow_test PROPERTIES FOLDER "Tests/App/VacancySearchWorkflow")

gtest_discover_tests(VacancySearchWorkflow_test PROPERTIES LABELS "unit;App")
