add_library(AbstractRequestStrategy
    AbstractRequestStrategy.h
    AbstractRequestStrategy.cpp
)

target_include_directories(AbstractRequestStrategy PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(AbstractRequestStrategy 
    PUBLIC 
        Qt6::Core
        AbstractNetworkManager
)

set_target_properties(AbstractRequestStrategy PROPERTIES FOLDER "src/Network/RequestStrategy")



add_library(LinkedInRequestStrategy
    LinkedInRequestStrategy.h
    LinkedInRequestStrategy.cpp
)

target_include_directories(LinkedInRequestStrategy PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(LinkedInRequestStrategy 
    PRIVATE
        AbstractRequestStrategy
        Qt6::Network
)

set_target_properties(LinkedInRequestStrategy PROPERTIES FOLDER "src/Network/RequestStrategy")
