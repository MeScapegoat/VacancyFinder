enable_testing()

#TODO: separate folder for each component, add_subdirectory

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

set_target_properties(test_helpers PROPERTIES FOLDER "Tests")



# General

add_executable(AppWorkflow_test
	app_workflow_test.cpp
	main_for_qt_tests.cpp
)

target_link_libraries(AppWorkflow_test
	PRIVATE
		App # TODO: change to AppController
		Network
		Parser
		LinkedInParserStrategy
		GTest::gtest
		Qt6::Widgets
		Qt6::Test
)

set_target_properties(AppWorkflow_test PROPERTIES FOLDER "Tests/General")



# App

add_executable(AppController_test
	App/AppController/AppController_test.cpp
)

target_link_libraries(AppController_test
	PRIVATE
		App
		GTest::gtest_main
		Qt6::Test
)

set_target_properties(AppController_test PROPERTIES FOLDER "Tests/App/AppController")



# Presentation

add_executable(ResultViewOptions_test
	Presentation/ResultViewOptions/ResultViewOptions_test.cpp
	main_for_qt_tests.cpp
)

target_link_libraries(ResultViewOptions_test
	PRIVATE
		ResultViewOptions
		test_helpers
)

set_target_properties(ResultViewOptions_test PROPERTIES FOLDER "Tests/Presentation/ResultViewOptions")



add_executable(QueryParameters_test
	Presentation/QueryParameters/QueryParameters_test.cpp
	main_for_qt_tests.cpp
)

target_link_libraries(QueryParameters_test
	PRIVATE
		QueryParameters
		test_helpers
)

set_target_properties(QueryParameters_test PROPERTIES FOLDER "Tests/Presentation/QueryParameters")



# Parsing

add_executable(Parser_test
	Parsing/Parser_test.cpp
)

target_link_libraries(Parser_test
	PRIVATE
		Parser
		GTest::gtest_main
		GTest::gmock
		Qt6::Test
)

set_target_properties(Parser_test PROPERTIES FOLDER "Tests/Parsing")



add_library(HtmlResources)

qt_add_resources(HtmlResources
    "html_resources"
    PREFIX "/"
    BASE Parsing/ParserStrategy/LinkedInParserStrategy/Html_resources
    FILES
        Parsing/ParserStrategy/LinkedInParserStrategy/Html_resources/html_1.html
		Parsing/ParserStrategy/LinkedInParserStrategy/Html_resources/html_2.html

		Parsing/ParserStrategy/LinkedInParserStrategy/Html_resources/json_1.html
        Parsing/ParserStrategy/LinkedInParserStrategy/Html_resources/json_2.html

		Parsing/ParserStrategy/LinkedInParserStrategy/Html_resources/normal_url_1.html
		Parsing/ParserStrategy/LinkedInParserStrategy/Html_resources/normal_url_2.html

		Parsing/ParserStrategy/LinkedInParserStrategy/Html_resources/alternate_url_1.html
        Parsing/ParserStrategy/LinkedInParserStrategy/Html_resources/alternate_url_2.html

		Parsing/ParserStrategy/LinkedInParserStrategy/Html_resources/full_briefs.html
        Parsing/ParserStrategy/LinkedInParserStrategy/Html_resources/partial_briefs.html
		Parsing/ParserStrategy/LinkedInParserStrategy/Html_resources/empty_briefs.html
	OUTPUT_TARGETS HtmlResources_output_targets
)

foreach(target IN LISTS HtmlResources_output_targets)
	set_target_properties(${target} PROPERTIES FOLDER "Tests/Parsing/ParserStrategy/HtmlResources")
endforeach()
set_target_properties(HtmlResources_other_files PROPERTIES FOLDER "Tests/Parsing/ParserStrategy/HtmlResources") # Target created by qt_add_resources, cringe alarm TODO (?)
set_target_properties(HtmlResources PROPERTIES FOLDER "Tests/Parsing/ParserStrategy/HtmlResources")



add_executable(LinkedInBriefVacancyExtractor_test
	Parsing/ParserStrategy/LinkedInParserStrategy/LinkedInBriefVacancyExtractor_test.cpp
)

target_link_libraries(LinkedInBriefVacancyExtractor_test
	PRIVATE
		LinkedInParserStrategy
		HtmlResources
		GTest::gtest_main
)

set_target_properties(LinkedInBriefVacancyExtractor_test PROPERTIES FOLDER "Tests/Parsing/ParserStrategy")



add_executable(LinkedInDetailsHtmlVacancyExtractor_test
	Parsing/ParserStrategy/LinkedInParserStrategy/LinkedInDetailsHtmlVacancyExtractor_test.cpp
)

target_link_libraries(LinkedInDetailsHtmlVacancyExtractor_test
	PRIVATE
		LinkedInParserStrategy
		HtmlResources
		GTest::gtest_main
)

set_target_properties(LinkedInDetailsHtmlVacancyExtractor_test PROPERTIES FOLDER "Tests/Parsing/ParserStrategy")



add_executable(LinkedInDetailsJsonVacancyExtractor_test
	Parsing/ParserStrategy/LinkedInParserStrategy/LinkedInDetailsJsonVacancyExtractor_test.cpp
)

target_link_libraries(LinkedInDetailsJsonVacancyExtractor_test
	PRIVATE
		LinkedInParserStrategy
		HtmlResources
		GTest::gtest_main
)

set_target_properties(LinkedInDetailsJsonVacancyExtractor_test PROPERTIES FOLDER "Tests/Parsing/ParserStrategy")



add_executable(LinkedInParserStrategy_test
	Parsing/ParserStrategy/LinkedInParserStrategy/LinkedInParserStrategy_test.cpp
)

target_link_libraries(LinkedInParserStrategy_test
	PRIVATE
		LinkedInParserStrategy
		HtmlResources
		GTest::gtest_main
)

set_target_properties(LinkedInParserStrategy_test PROPERTIES FOLDER "Tests/Parsing/ParserStrategy")



include(GoogleTest)

gtest_discover_tests(AppWorkflow_test)
gtest_discover_tests(AppController_test)
gtest_discover_tests(ResultViewOptions_test)
gtest_discover_tests(QueryParameters_test)
gtest_discover_tests(LinkedInBriefVacancyExtractor_test)
gtest_discover_tests(LinkedInDetailsHtmlVacancyExtractor_test)
gtest_discover_tests(LinkedInDetailsJsonVacancyExtractor_test)
gtest_discover_tests(Parser_test)