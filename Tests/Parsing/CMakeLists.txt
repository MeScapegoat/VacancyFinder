add_executable(Parser_test
	Parser_test.cpp
)

target_link_libraries(Parser_test
	PRIVATE
		Parser
		GTest::gtest_main
		GTest::gmock
		Qt6::Test
)

set_target_properties(Parser_test PROPERTIES FOLDER "Tests/Parsing")

gtest_discover_tests(Parser_test)



add_library(html_resources)

qt_add_resources(html_resources
    "html_resources"
    PREFIX "/"
    BASE ParserStrategy/LinkedInParserStrategy/html_resources
    FILES
        ParserStrategy/LinkedInParserStrategy/html_resources/html_1.html
		ParserStrategy/LinkedInParserStrategy/html_resources/html_2.html

		ParserStrategy/LinkedInParserStrategy/html_resources/json_1.html
        ParserStrategy/LinkedInParserStrategy/html_resources/json_2.html

		ParserStrategy/LinkedInParserStrategy/html_resources/normal_url_1.html
		ParserStrategy/LinkedInParserStrategy/html_resources/normal_url_2.html

		ParserStrategy/LinkedInParserStrategy/html_resources/alternate_url_1.html
        ParserStrategy/LinkedInParserStrategy/html_resources/alternate_url_2.html

		ParserStrategy/LinkedInParserStrategy/html_resources/full_briefs.html
        ParserStrategy/LinkedInParserStrategy/html_resources/partial_briefs.html
		ParserStrategy/LinkedInParserStrategy/html_resources/empty_briefs.html
	OUTPUT_TARGETS html_resources_output_targets
)

foreach(target IN LISTS html_resources_output_targets)
	set_target_properties(${target}				 PROPERTIES FOLDER "Tests/Parsing/ParserStrategy/html_resources")
endforeach()
set_target_properties(html_resources_other_files PROPERTIES FOLDER "Tests/Parsing/ParserStrategy/html_resources") # Target created by qt_add_resources, cringe alarm TODO (?)
set_target_properties(html_resources			 PROPERTIES FOLDER "Tests/Parsing/ParserStrategy/html_resources")



add_executable(LinkedInBriefVacancyExtractor_test
	ParserStrategy/LinkedInParserStrategy/LinkedInBriefVacancyExtractor_test.cpp
)

target_link_libraries(LinkedInBriefVacancyExtractor_test
	PRIVATE
		LinkedInParserStrategy
		html_resources
		GTest::gtest_main
)

set_target_properties(LinkedInBriefVacancyExtractor_test PROPERTIES FOLDER "Tests/Parsing/ParserStrategy")

gtest_discover_tests(LinkedInBriefVacancyExtractor_test)



add_executable(LinkedInDetailsHtmlVacancyExtractor_test
	ParserStrategy/LinkedInParserStrategy/LinkedInDetailsHtmlVacancyExtractor_test.cpp
)

target_link_libraries(LinkedInDetailsHtmlVacancyExtractor_test
	PRIVATE
		LinkedInParserStrategy
		html_resources
		GTest::gtest_main
)

set_target_properties(LinkedInDetailsHtmlVacancyExtractor_test PROPERTIES FOLDER "Tests/Parsing/ParserStrategy")

gtest_discover_tests(LinkedInDetailsHtmlVacancyExtractor_test)



add_executable(LinkedInDetailsJsonVacancyExtractor_test
	ParserStrategy/LinkedInParserStrategy/LinkedInDetailsJsonVacancyExtractor_test.cpp
)

target_link_libraries(LinkedInDetailsJsonVacancyExtractor_test
	PRIVATE
		LinkedInParserStrategy
		html_resources
		GTest::gtest_main
)

set_target_properties(LinkedInDetailsJsonVacancyExtractor_test PROPERTIES FOLDER "Tests/Parsing/ParserStrategy")

gtest_discover_tests(LinkedInDetailsJsonVacancyExtractor_test)



add_executable(LinkedInParserStrategy_test
	ParserStrategy/LinkedInParserStrategy/LinkedInParserStrategy_test.cpp
)

target_link_libraries(LinkedInParserStrategy_test
	PRIVATE
		LinkedInParserStrategy
		html_resources
		GTest::gtest_main
)

set_target_properties(LinkedInParserStrategy_test PROPERTIES FOLDER "Tests/Parsing/ParserStrategy")

gtest_discover_tests(LinkedInParserStrategy_test)