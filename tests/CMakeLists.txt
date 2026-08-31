set(BENCHMARK_TARGET_NAME "RequestManagerBenchmark")
set(TEST_SOURCES_DIR "RequestManagerBenchmark")


add_executable(${BENCHMARK_TARGET_NAME}
    ${TEST_SOURCES_DIR}/benchmark.cpp
    ${NETWORK_SOURCES}
)

target_include_directories(${BENCHMARK_TARGET_NAME}
    PRIVATE
        ${SOURCES_DIR_PATH}
)

target_link_libraries(${BENCHMARK_TARGET_NAME}
    PRIVATE
        Qt6::Core
        Qt6::Network
)


set(ANALYZER_TARGET_NAME "BenchmarkAnalyzer")

add_executable(${ANALYZER_TARGET_NAME}
    ${TEST_SOURCES_DIR}/benchmarkAnalyzer.cpp
)

target_link_libraries(${ANALYZER_TARGET_NAME}
    PRIVATE
        Qt6::Core
)