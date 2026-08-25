cmake_minimum_required(VERSION 3.20)

project(LinkedIn_search_app
        VERSION 1.0
        LANGUAGES CXX
        )

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

add_executable(LinkedIn_search_app)

target_sources(LinkedIn_search_app 
                PUBLIC src/main.cpp 
                PUBLIC src/core/App.cpp 
                PUBLIC src/core/App.h
                PUBLIC src/core/Vacancy.cpp
                PUBLIC src/core/Vacancy.h
                PUBLIC src/core/Logger.h
                PUBLIC src/core/Logger.cpp

                PUBLIC src/network/NetworkManager.h
                PUBLIC src/network/NetworkManager.cpp
                PUBLIC src/network/HTMLparser.h
                PUBLIC src/network/HTMLparser.cpp
                
                PUBLIC src/gui/GUI.h
                PUBLIC src/gui/GUI.cpp
                PUBLIC src/gui/MainWindow.h
                PUBLIC src/gui/MainWindow.cpp
                PUBLIC src/gui/ProgressDialog.h
                PUBLIC src/gui/ProgressDialog.cpp) # TODO change this

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