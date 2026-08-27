set(TEST_TARGET_NAME "RequestManagerBenchmark")
set(TEST_SOURCES_DIR "RequestManagerBenchmark")


add_executable(${TEST_TARGET_NAME}
    ${TEST_SOURCES_DIR}/main.cpp
    ${NETWORK_SOURCES}
)

target_include_directories(${TEST_TARGET_NAME}
    PRIVATE
        ${SOURCES_DIR_PATH}
)

target_link_libraries(${TEST_TARGET_NAME}
    PRIVATE
        Qt6::Core
        Qt6::Network
)