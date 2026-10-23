add_executable(LinkedInSearchUrlProvider_test
	LinkedInSearchUrlProvider_test.cpp
)

target_link_libraries(LinkedInSearchUrlProvider_test
	PRIVATE
		LinkedInSearchUrlProvider
		GTest::gtest_main
		Qt6::Core
)

set_target_properties(LinkedInSearchUrlProvider_test PROPERTIES FOLDER "Tests/Network/SearchUrlProvider")

gtest_discover_tests(LinkedInSearchUrlProvider_test PROPERTIES LABELS "unit;Network")
