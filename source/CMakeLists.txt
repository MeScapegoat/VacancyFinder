cmake_minimum_required(VERSION 3.20)
if (WIN32)
        message("Configuring for Windows...")
        
        add_compile_options(/Wall /permissive-)
elseif (UNIX AND NOT APPLE)
        message("Configuring for Linux...")
        
        add_compile_options(-O2 -Wall -Wextra -Wpedantic)
else()
        message("The app wasn't tested for ${CMAKE_HOST_SYSTEM_NAME}")
endif()
set(CMAKE_AUTOMOC ON)         

project(LinkedIn_search_app
        VERSION 1.0
        LANGUAGES CXX
        )

add_executable(LinkedIn_search_app)

target_sources(LinkedIn_search_app 
                PUBLIC main.cpp 
                PUBLIC App.cpp 
                PUBLIC App.h
                PUBLIC Vacancy.cpp
                PUBLIC Vacancy.h
                PUBLIC NetworkManager.h
                PUBLIC NetworkManager.cpp
                PUBLIC HTMLparser.h
                PUBLIC HTMLparser.cpp
                PUBLIC Logger.h
                PUBLIC Logger.cpp
                PUBLIC GUI.h
                PUBLIC GUI.cpp
                PUBLIC MainWindow.h
                PUBLIC MainWindow.cpp) # TODO change this

find_package(Qt6 COMPONENTS Core Network Widgets REQUIRED)
target_link_libraries(LinkedIn_search_app 
                      PRIVATE Qt6::Core Qt6::Network Qt6::Widgets)

target_include_directories(LinkedIn_search_app 
                           PUBLIC source)

target_compile_definitions(LinkedIn_search_app
                           PUBLIC -DQT_MESSAGELOGCONTEXT)

message(STATUS "Generator: ${CMAKE_GENERATOR}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "C++ Compiler Flags: ${CMAKE_CXX_FLAGS}")