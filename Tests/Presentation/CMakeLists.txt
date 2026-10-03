add_executable(ResultViewOptions_test
	ResultViewOptions/ResultViewOptions_test.cpp
)

target_link_libraries(ResultViewOptions_test
	PRIVATE
		main_for_qt_tests
		ResultViewOptions
		test_helpers
)

set_target_properties(ResultViewOptions_test PROPERTIES FOLDER "Tests/Presentation")

gtest_discover_tests(ResultViewOptions_test)



add_executable(QueryParameters_test
	QueryParameters/QueryParameters_test.cpp
)

target_link_libraries(QueryParameters_test
	PRIVATE
		main_for_qt_tests
		QueryParameters
		test_helpers
)

set_target_properties(QueryParameters_test PROPERTIES FOLDER "Tests/Presentation")

gtest_discover_tests(QueryParameters_test)