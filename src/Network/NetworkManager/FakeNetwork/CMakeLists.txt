add_library(FakeNetworkReply
    FakeNetworkReply.h
    FakeNetworkReply.cpp
)

target_include_directories(FakeNetworkReply PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(FakeNetworkReply 
    PUBLIC
        Qt6::Network
)

set_target_properties(FakeNetworkReply PROPERTIES FOLDER "src/Network/NetworkManager/FakeNetwork")



add_library(FakeNetworkManager
    FakeNetworkManager.h
    FakeNetworkManager.cpp
)

target_include_directories(FakeNetworkManager PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(FakeNetworkManager 
    PUBLIC
        Qt6::Network
        AbstractNetworkManager
    PRIVATE
        FakeNetworkReply
)

set_target_properties(FakeNetworkManager PROPERTIES FOLDER "src/Network/NetworkManager/FakeNetwork")