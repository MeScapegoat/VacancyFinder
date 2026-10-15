add_library(Utils
    Observable.hpp
    Observable.cpp

    Utils.hpp
    Utils.cpp)

target_include_directories(Utils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Utils PUBLIC Qt6::Core)




add_library(ConnectionGroup
    ConnectionGroup.h
    ConnectionGroup.cpp
)

target_include_directories(ConnectionGroup PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ConnectionGroup PUBLIC Qt6::Core)



add_library(CallableComboBox
    CallableComboBox.hpp
)

target_include_directories(CallableComboBox PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(CallableComboBox PUBLIC Qt6::Core)