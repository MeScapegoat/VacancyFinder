add_library(AbstractSearchUrlProvider INTERFACE
    AbstractSearchUrlProvider.h)

target_include_directories(AbstractSearchUrlProvider INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(AbstractSearchUrlProvider
    INTERFACE
        Qt6::Core
)

set_target_properties(AbstractSearchUrlProvider PROPERTIES FOLDER "src/Network/SearchUrlProvider")



add_library(LinkedInSearchUrlProvider
    LinkedInSearchUrlProvider.h
    LinkedInSearchUrlProvider.cpp
)

target_include_directories(LinkedInSearchUrlProvider PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(LinkedInSearchUrlProvider
    PUBLIC
        AbstractSearchUrlProvider
)

set_target_properties(LinkedInSearchUrlProvider PROPERTIES FOLDER "src/Network/SearchUrlProvider")
