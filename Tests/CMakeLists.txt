include(CTest) 
enable_testing() 
include(GoogleTest)

# Helpers

add_library(QtTestPrinters INTERFACE
	QtTestPrinters.h)

target_link_libraries(QtTestPrinters
	INTERFACE
		Qt6::Core
)

target_include_directories(QtTestPrinters INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

link_libraries(QtTestPrinters)

target_precompile_headers(QtTestPrinters INTERFACE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/QtTestPrinters.h>"
)

set_target_properties(QtTestPrinters PROPERTIES FOLDER "Tests/Support")



# main.cpp for qt tests

add_library(main_for_qt_tests
	main_for_qt_tests.cpp)

target_link_libraries(main_for_qt_tests
	PUBLIC
		GTest::gtest
		Qt6::Widgets
)

set_target_properties(main_for_qt_tests PROPERTIES FOLDER "Tests/Support")



# General

add_executable(VacancySearchWorkflowIntegration_test
	App/VacancySearchWorkflow/VacancySearchWorkflowIntegration_test.cpp
)

target_link_libraries(VacancySearchWorkflowIntegration_test
	PRIVATE
		main_for_qt_tests
		VacancySearchWorkflow
		LinkedInSearchUrlProvider
		NetworkManager
		LinkedInParserStrategy
		LinkedInRequestStrategy
		Parser
		RequestManager
		GTest::gtest
		Qt6::Test
)

set_target_properties(VacancySearchWorkflowIntegration_test PROPERTIES FOLDER "Tests/App/VacancySearchWorkflow/Integration")

gtest_discover_tests(VacancySearchWorkflowIntegration_test PROPERTIES LABELS integration)



add_subdirectory(App)
add_subdirectory(Presentation)
add_subdirectory(Parsing)
add_subdirectory(Utils)
add_subdirectory(Network)
add_subdirectory(Domain)
