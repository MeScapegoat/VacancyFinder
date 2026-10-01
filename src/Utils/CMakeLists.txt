add_library(Utils
    Observable.hpp
    Observable.cpp

    Utils.hpp
    Utils.cpp)

target_include_directories(Utils PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Utils PUBLIC Qt6::Core)