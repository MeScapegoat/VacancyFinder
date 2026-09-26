enable_testing()


# Presentation

add_executable(ResultViewOptions_tests
	Presentation/ResultViewOptions/ResultViewOptions_tests.cpp
	Presentation/ResultViewOptions/main_test.cpp
)

target_link_libraries(ResultViewOptions_tests
	PRIVATE
		ResultViewOptions
		GTest::gtest
		Qt6::Test
)

set_target_properties(ResultViewOptions_tests PROPERTIES FOLDER "Tests/ResultViewOptions")

include(GoogleTest)

gtest_discover_tests(ResultViewOptions_tests)