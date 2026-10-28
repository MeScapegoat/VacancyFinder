add_executable(VacancyOperations_test
	VacancyOperations_test.cpp
)

target_link_libraries(VacancyOperations_test
	PRIVATE
		main_for_qt_tests
		VacancyOperations
		Qt::Test
)

set_target_properties(VacancyOperations_test PROPERTIES FOLDER "Tests/Domain")

gtest_discover_tests(VacancyOperations_test PROPERTIES LABELS "unit;Domain")



add_executable(Vacancy_test
	Vacancy_test.cpp
)

target_link_libraries(Vacancy_test
	PRIVATE
		Domain
		GTest::gtest_main
)

set_target_properties(Vacancy_test PROPERTIES FOLDER "Tests/Domain")

gtest_discover_tests(Vacancy_test PROPERTIES LABELS "unit;Domain")



add_executable(VacancyExtractor_test
	VacancyExtractor_test.cpp
)

target_link_libraries(VacancyExtractor_test
	PRIVATE
		Domain
		GTest::gtest_main
)

set_target_properties(VacancyExtractor_test PROPERTIES FOLDER "Tests/Domain")

gtest_discover_tests(VacancyExtractor_test PROPERTIES LABELS "unit;Domain")
