add_library(Core
    Logger.h
    Logger.cpp
)

target_include_directories(Core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Core
    PUBLIC 
        Qt6::Core
)



add_library(FunctionTypes
    FunctionTypes.h
)

target_include_directories(FunctionTypes INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)