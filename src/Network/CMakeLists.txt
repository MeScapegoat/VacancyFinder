add_subdirectory(NetworkManager)
add_subdirectory(RequestStrategy)
add_subdirectory(SearchUrlProvider)



add_library(RequestManager
    RequestManager.h
    RequestManager.cpp
)

target_include_directories(RequestManager PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(RequestManager 
    PUBLIC 
        Qt6::Core
    PRIVATE
        AbstractRequestStrategy
)

set_target_properties(RequestManager PROPERTIES FOLDER "Network")
