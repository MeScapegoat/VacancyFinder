add_executable(ResultViewOptions_test
	ResultViewOptions/ResultViewOptions_test.cpp
)

target_link_libraries(ResultViewOptions_test
	PRIVATE
		main_for_qt_tests
		ResultViewOptions
		QtTestPrinters
		Qt::Test
)

set_target_properties(ResultViewOptions_test PROPERTIES FOLDER "Tests/Presentation")

gtest_discover_tests(ResultViewOptions_test PROPERTIES LABELS "unit;Presentation")



add_executable(QueryParameters_test
	QueryParameters/QueryParameters_test.cpp
)

target_link_libraries(QueryParameters_test
	PRIVATE
		main_for_qt_tests
		QueryParameters
		QtTestPrinters
		Qt::Test
)

set_target_properties(QueryParameters_test PROPERTIES FOLDER "Tests/Presentation")

gtest_discover_tests(QueryParameters_test PROPERTIES LABELS "unit;Presentation")



add_executable(VacancyBrowser_test
	VacancyBrowser/VacancyBrowserModel_test.cpp
	VacancyBrowser/VacancyBrowserPresenter_test.cpp
)

target_link_libraries(VacancyBrowser_test
	PRIVATE
		main_for_qt_tests
		VacancyBrowser
		QtTestPrinters
)

set_target_properties(VacancyBrowser_test PROPERTIES FOLDER "Tests/Presentation")

gtest_discover_tests(VacancyBrowser_test PROPERTIES LABELS "unit;Presentation" ENVIRONMENT "QT_QPA_PLATFORM=offscreen")
