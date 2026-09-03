cmake_minimum_required(VERSION 4.4)


# Project configuration

set(PROJECT_NAME "LinkedIn_Job_Finder")
set(PROJECT_VERSION "0.0.1")

set(SOURCES_DIR_FOLDER "src")
set(SOURCES_DIR_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCES_DIR_FOLDER}")

set(CORE_SOURCES_DIR_FOLDER "core")
set(CORE_SOURCES "")

set(NETWORK_SOURCES_DIR_FOLDER "network")
set(NETWORK_SOURCES "")

set(PARSER_SOURCES_DIR_FOLDER "parser")
set(PARSER_SOURCES "")

set(GUI_SOURCES_DIR_FOLDER "gui")
set(GUI_SOURCES "")

set(MODEL_SOURCES_DIR_FOLDER "model")
set(MODEL_SOURCES "")

set(UTILS_SOURCES_DIR_FOLDER "utils")
set(UTILS_SOURCES "")

set(DATA_SOURCES_DIR_FOLDER "data")
set(DATA_SOURCES "")

set(SOURCES_SUPPORTED_EXTENSIONS
    "*.cpp"
    "*.h"
)

configure_file(
    ${CMAKE_SOURCE_DIR}/log_rules.ini
    ${CMAKE_BINARY_DIR}/log_rules.ini
    COPYONLY
)

# Project

project(${PROJECT_NAME}
    VERSION ${PROJECT_VERSION}
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Qt

find_package(Qt6 REQUIRED COMPONENTS 
            Core
            Network
            Widgets)

set(CMAKE_AUTOMOC ON)


# Collect sources files

foreach(EXTENSION IN LISTS SOURCES_SUPPORTED_EXTENSIONS)
    file(GLOB_RECURSE 
        FILES
        "${SOURCES_DIR_PATH}/${CORE_SOURCES_DIR_FOLDER}/${EXTENSION}"
    )

    list(APPEND CORE_SOURCES ${FILES})
endforeach()

foreach(EXTENSION IN LISTS SOURCES_SUPPORTED_EXTENSIONS)
    file(GLOB_RECURSE 
        FILES
        "${SOURCES_DIR_PATH}/${NETWORK_SOURCES_DIR_FOLDER}/${EXTENSION}"
    )

    list(APPEND NETWORK_SOURCES ${FILES})
endforeach()

foreach(EXTENSION IN LISTS SOURCES_SUPPORTED_EXTENSIONS)
    file(GLOB_RECURSE 
        FILES
        "${SOURCES_DIR_PATH}/${PARSER_SOURCES_DIR_FOLDER}/${EXTENSION}"
    )

    list(APPEND PARSER_SOURCES ${FILES})
endforeach()

foreach(EXTENSION IN LISTS SOURCES_SUPPORTED_EXTENSIONS)
    file(GLOB_RECURSE 
        FILES
        "${SOURCES_DIR_PATH}/${GUI_SOURCES_DIR_FOLDER}/${EXTENSION}"
    )

    list(APPEND GUI_SOURCES ${FILES})
endforeach()

foreach(EXTENSION IN LISTS SOURCES_SUPPORTED_EXTENSIONS)
    file(GLOB_RECURSE 
        FILES
        "${SOURCES_DIR_PATH}/${MODEL_SOURCES_DIR_FOLDER}/${EXTENSION}"
    )

    list(APPEND MODEL_SOURCES ${FILES})
endforeach()

foreach(EXTENSION IN LISTS SOURCES_SUPPORTED_EXTENSIONS)
    file(GLOB_RECURSE 
        FILES
        "${SOURCES_DIR_PATH}/${UTILS_SOURCES_DIR_FOLDER}/${EXTENSION}"
    )

    list(APPEND UTILS_SOURCES ${FILES})
endforeach()

foreach(EXTENSION IN LISTS SOURCES_SUPPORTED_EXTENSIONS)
    file(GLOB_RECURSE 
        FILES
        "${SOURCES_DIR_PATH}/${DATA_SOURCES_DIR_FOLDER}/${EXTENSION}"
    )

    list(APPEND DATA_SOURCES ${FILES})
endforeach()


# Target

add_executable(${PROJECT_NAME} 
               ${CORE_SOURCES}
               ${NETWORK_SOURCES}
               ${PARSER_SOURCES}
               ${GUI_SOURCES}
               ${MODEL_SOURCES}
               ${UTILS_SOURCES}
               ${DATA_SOURCES}
               ${SOURCES_DIR_PATH}/main.cpp       
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        Qt6::Core
        Qt6::Network
        Qt6::Widgets
)

add_subdirectory(tests)