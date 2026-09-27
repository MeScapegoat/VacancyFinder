enable_testing()


# Presentation

add_executable(ResultViewOptions_test
	Presentation/ResultViewOptions/ResultViewOptions_test.cpp
	Presentation/ResultViewOptions/main_test.cpp
)

target_link_libraries(ResultViewOptions_test
	PRIVATE
		ResultViewOptions
		GTest::gtest
		Qt6::Test
)

set_target_properties(ResultViewOptions_test PROPERTIES FOLDER "Tests/ResultViewOptions")

include(GoogleTest)

gtest_discover_tests(ResultViewOptions_test)