enable_testing()

# Helpers

add_library(test_helpers INTERFACE test_helpers.hpp)

target_link_libraries(test_helpers
	INTERFACE
		GTest::gtest
		Utils
		Qt6::Widget
)

set_target_properties(test_helpers PROPERTIES FOLDER "Tests")

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


add_executable(QueryParameters_test
	Presentation/QueryParameters/QueryParameters_test.cpp
	Presentation/QueryParameters/main_test.cpp
)

target_include_directories(QueryParameters_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(QueryParameters_test
	PRIVATE
		QueryParameters
		GTest::gtest
		Qt6::Test
	INTERFACE
		test_helpers
)

set_target_properties(QueryParameters_test PROPERTIES FOLDER "Tests/QueryParameters")

include(GoogleTest)

gtest_discover_tests(ResultViewOptions_test)
gtest_discover_tests(QueryParameters_test)