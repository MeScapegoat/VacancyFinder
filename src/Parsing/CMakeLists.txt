add_library(Parsing
    HTMLParser.h
    HTMLParser.cpp
)

target_include_directories(Parsing PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Parsing 
    PUBLIC 
        Qt6::Core
    PRIVATE
        Domain
)