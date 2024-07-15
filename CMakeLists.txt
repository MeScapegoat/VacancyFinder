cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_PREFIX_PATH "C:/Qt/6.5.0/mingw_64") # TODO use more generous approach

project(LinkedInParser
        VERSION 1.0
        LANGUAGES CXX
        )

find_package(Qt6 COMPONENTS Core Network REQUIRED)

set(CMAKE_AUTOMOC ON)

add_executable(LinkedInParser)

target_sources(LinkedInParser 
                PUBLIC source/main.cpp 
                PUBLIC source/LinkedApp.cpp 
                PUBLIC source/LinkedApp.h
                PUBLIC source/LinkedVacancy.cpp
                PUBLIC source/LinkedVacancy.h) # TODO change this

target_link_libraries(LinkedInParser PRIVATE Qt6::Core Qt6::Network)
target_include_directories(LinkedInParser PUBLIC source)
message(STATUS "")
message(STATUS "INFO:")
message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message(STATUS "Using generator: ${CMAKE_GENERATOR}")
message(STATUS "Using compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "Qt6_DIR: ${Qt6_DIR}")
message(STATUS "Qt6Core_DIR: ${Qt6Core_DIR}")
message(STATUS "Qt6Network_DIR: ${Qt6Network_DIR}")
message(STATUS "")
