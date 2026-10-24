include(CTest) 
enable_testing() 
include(GoogleTest)

# Helpers

add_library(test_helpers 
	INTERFACE
		test_utils.hpp
		test_helpers.hpp)

target_link_libraries(test_helpers
	INTERFACE
		Utils
		GTest::gtest
		Qt6::Test
		Qt6::Widgets
)

target_include_directories(test_helpers INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(test_helpers PROPERTIES FOLDER "Tests/Support")



add_library(QtTestPrinters
	QtTestPrinters.h)

target_link_libraries(QtTestPrinters
	INTERFACE
		Qt6::Core
)

target_include_directories(QtTestPrinters INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
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

add_executable(app_workflow_test
	app_workflow_test.cpp
	
)

target_link_libraries(app_workflow_test
	PRIVATE
		main_for_qt_tests
		WorkCoordinator
		NetworkManager
		RequestManager
		Parser
		LinkedInParserStrategy
		LinkedInRequestStrategy
		GTest::gtest
		Qt6::Widgets
		Qt6::Test
)

set_target_properties(app_workflow_test PROPERTIES FOLDER "Tests/App/Integration")

gtest_discover_tests(app_workflow_test PROPERTIES LABELS integration)



add_subdirectory(App)
add_subdirectory(Presentation)
add_subdirectory(Parsing)
add_subdirectory(Utils)
add_subdirectory(Network)
add_subdirectory(Domain)
