cmake_minimum_required(VERSION 4.3)

set(SOURCE_FOLDER_NAME "src")

project(VacancyFinder
    VERSION 0.0.1
    LANGUAGES CXX
)

configure_file(
    ${CMAKE_SOURCE_DIR}/log_rules.ini
    ${CMAKE_BINARY_DIR}/log_rules.ini
    COPYONLY
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 
    REQUIRED COMPONENTS 
        Core
        Network
        Widgets
        Test)

qt_standard_project_setup()

qt_add_executable(VacancyFinderApp ${SOURCE_FOLDER_NAME}/main.cpp)

add_subdirectory(${SOURCE_FOLDER_NAME}/Core)
add_subdirectory(${SOURCE_FOLDER_NAME}/Domain)
add_subdirectory(${SOURCE_FOLDER_NAME}/Network)
add_subdirectory(${SOURCE_FOLDER_NAME}/Presentation)
add_subdirectory(${SOURCE_FOLDER_NAME}/UI)
add_subdirectory(${SOURCE_FOLDER_NAME}/Utils)

add_subdirectory(${SOURCE_FOLDER_NAME}/Parsing)

add_subdirectory(${SOURCE_FOLDER_NAME}/App)

target_link_libraries(VacancyFinderApp
    PRIVATE
        VacancyFinder
        Core
        Qt6::Widgets
)


include(FetchContent)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.18.0
    FIND_PACKAGE_ARGS
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

if(TARGET gtest)
    set_target_properties(gtest PROPERTIES FOLDER "Tests/GoogleTest")
endif()

if(TARGET gtest_main)
    set_target_properties(gtest_main PROPERTIES FOLDER "Tests/GoogleTest")
endif()

if(TARGET gmock)
    set_target_properties(gmock PROPERTIES FOLDER "Tests/GoogleTest")
endif()

if(TARGET gmock_main)
    set_target_properties(gmock_main PROPERTIES FOLDER "Tests/GoogleTest")
endif()


add_subdirectory(Benchmarks)
add_subdirectory(Tests)
