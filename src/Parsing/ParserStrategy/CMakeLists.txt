add_library(AbstractParserStrategy
    AbstractParserStrategy.h
)

target_include_directories(AbstractParserStrategy INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(AbstractParserStrategy 
    INTERFACE
        Domain
        Qt6::Core
)

set_target_properties(AbstractParserStrategy PROPERTIES FOLDER "Parsing")



add_library(LinkedInParserStrategy
    LinkedInParserStrategy/LinkedInParserStrategy.h
    LinkedInParserStrategy/LinkedInParserStrategy.cpp
)

target_include_directories(LinkedInParserStrategy PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(LinkedInParserStrategy 
    PUBLIC
        AbstractParserStrategy
    PRIVATE
        Utils
)

set_target_properties(LinkedInParserStrategy PROPERTIES FOLDER "Parsing")



add_library(LinkedInPageCollector
    LinkedInParserStrategy/LinkedInPageCollector.h
    LinkedInParserStrategy/LinkedInPageCollector.cpp
)

target_include_directories(LinkedInPageCollector PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(LinkedInPageCollector 
    PUBLIC
        LinkedInParserStrategy
    PRIVATE
        Utils
)

set_target_properties(LinkedInPageCollector PROPERTIES FOLDER "Parsing")