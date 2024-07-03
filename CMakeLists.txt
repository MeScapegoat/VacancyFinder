cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_COMPILER "g++")

project(LinkedInParser
        VERSION 1.0
        LANGUAGES CXX
        )

message(STATUS "Using compiler: ${CMAKE_CXX_COMPILER}")

add_executable(LinkedInParser source/main.cpp)