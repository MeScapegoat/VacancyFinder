add_subdirectory(NetworkManager)



add_library(Network
    RequestManager.h
    RequestManager.cpp
)

target_include_directories(Network PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Network 
    PUBLIC 
        Qt6::Core
    PRIVATE
        Qt6::Network
)

set_target_properties(Network PROPERTIES FOLDER "Network")