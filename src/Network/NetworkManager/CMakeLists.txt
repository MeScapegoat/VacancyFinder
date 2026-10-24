add_library(AbstractNetworkManager
    AbstractNetworkManager.h
)

target_include_directories(AbstractNetworkManager PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(AbstractNetworkManager 
    PUBLIC 
        Qt6::Core        
)

set_target_properties(AbstractNetworkManager PROPERTIES FOLDER "src/Network/NetworkManager")



add_library(NetworkManager
    NetworkManager.h
    NetworkManager.cpp
)

target_include_directories(NetworkManager PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(NetworkManager 
    PUBLIC
        Qt6::Network
    PRIVATE 
        AbstractNetworkManager
)

set_target_properties(NetworkManager PROPERTIES FOLDER "src/Network/NetworkManager")



add_subdirectory(FakeNetwork)