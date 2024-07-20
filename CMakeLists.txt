cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_PREFIX_PATH "C:/Qt/6.5.0/mingw_64") # TODO use more generous approach
set(CMAKE_AUTOMOC ON)

add_compile_options(-O2 -Wall -Wextra -Wpedantic)         

project(LinkedIn_search_app
        VERSION 1.0
        LANGUAGES CXX
        )

add_executable(LinkedIn_search_app)

target_sources(LinkedIn_search_app 
                PUBLIC source/main.cpp 
                PUBLIC source/App.cpp 
                PUBLIC source/App.h
                PUBLIC source/Vacancy.cpp
                PUBLIC source/Vacancy.h
                PUBLIC source/NetworkManager.h
                PUBLIC source/NetworkManager.cpp
                PUBLIC source/HTMLparser.h
                PUBLIC source/HTMLparser.cpp) # TODO change this

find_package(Qt6 COMPONENTS Core Network REQUIRED)
target_link_libraries(LinkedIn_search_app 
                      PRIVATE Qt6::Core Qt6::Network)

target_include_directories(LinkedIn_search_app 
                           PUBLIC source)
             
message(STATUS "")
message(STATUS "INFO:")
message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message(STATUS "Using generator: ${CMAKE_GENERATOR}")
message(STATUS "Using compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Qt6_DIR: ${Qt6_DIR}")
message(STATUS "")
