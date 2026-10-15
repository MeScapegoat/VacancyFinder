add_executable(VacancyOperations_test
	VacancyOperations_test.cpp
)

target_link_libraries(VacancyOperations_test
	PRIVATE
		main_for_qt_tests
		VacancyOperations
		QtTestPrinters
		Qt::Test
)

set_target_properties(VacancyOperations_test PROPERTIES FOLDER "Tests/Domain")

gtest_discover_tests(VacancyOperations_test)