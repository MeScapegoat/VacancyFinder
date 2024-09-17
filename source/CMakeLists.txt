cmake_minimum_required(VERSION 3.20)
if (WIN32)
        message("Configuring for Windows...")
        
        add_compile_options(/Wall /permissive-)
elseif (UNIX AND NOT APPLE)
        message("Configuring for Linux...")
        
        add_compile_options(-O2 -Wall -Wextra -Wpedantic -fdiagnostics-color=always)
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
                PUBLIC core/App.cpp 
                PUBLIC core/App.h
                PUBLIC core/Vacancy.cpp
                PUBLIC core/Vacancy.h
                PUBLIC core/Logger.h
                PUBLIC core/Logger.cpp

                PUBLIC network/NetworkManager.h
                PUBLIC network/NetworkManager.cpp
                PUBLIC network/HTMLparser.h
                PUBLIC network/HTMLparser.cpp
                
                PUBLIC gui/GUI.h
                PUBLIC gui/GUI.cpp
                PUBLIC gui/MainWindow.h
                PUBLIC gui/MainWindow.cpp
                PUBLIC gui/ProgressDialog.h
                PUBLIC gui/ProgressDialog.cpp) # TODO change this

find_package(Qt6 COMPONENTS Core Network Widgets REQUIRED)
target_link_libraries(LinkedIn_search_app 
                      PRIVATE Qt6::Core Qt6::Network Qt6::Widgets)

target_include_directories(LinkedIn_search_app 
                           PUBLIC source)

target_compile_definitions(LinkedIn_search_app
                           PUBLIC -DQT_MESSAGELOGCONTEXT)

message(DEBUG "Generator: ${CMAKE_GENERATOR}")
message(DEBUG "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(DEBUG "C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(DEBUG "C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")
get_target_property(TARGET_FLAGS LinkedIn_search_app COMPILE_OPTIONS)
message(DEBUG "C++ Compiler Flags: ${TARGET_FLAGS}")