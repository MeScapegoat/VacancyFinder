cmake_minimum_required(VERSION 3.20)

project(LinkedInParser
        VERSION 1.0
        DESCRIPTION "Nice search, linkedIn!"
        LANGUAGES CXX
        )

add_executable(LinkedInParser source/main.cpp)

target_include_directories(LinkedInParser PUBLIC source)