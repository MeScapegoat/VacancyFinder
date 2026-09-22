set(TEST_SOURCES_DIR "RequestManagerBenchmark")


# RequestManagerBenchmark

add_executable(RequestManagerBenchmark EXCLUDE_FROM_ALL
    ${TEST_SOURCES_DIR}/benchmark.cpp
    ${NETWORK_SOURCES}
)

target_include_directories(RequestManagerBenchmark
    PRIVATE
        ${SOURCES_DIR_PATH}
)

target_link_libraries(RequestManagerBenchmark
    PRIVATE
        Qt6::Core
        Qt6::Network
)


# BenchmarkAnalyzer

add_executable(BenchmarkAnalyzer EXCLUDE_FROM_ALL
    ${TEST_SOURCES_DIR}/benchmarkAnalyzer.cpp
)

target_link_libraries(BenchmarkAnalyzer
    PRIVATE
        Qt6::Core
)