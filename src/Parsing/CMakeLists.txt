add_subdirectory(ParserStrategy)

add_library(Parser
    Parser.h
    Parser.cpp
)

target_include_directories(Parser PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Parser 
    PUBLIC 
        Qt6::Core
        Domain
    PRIVATE
        AbstractParserStrategy
        Utils
)

set_target_properties(Parser PROPERTIES FOLDER "Parsing")