set(REQUEST_MANAGER_BENCHMARK_SOURCES_FOLDER_NAME "RequestManagerBenchmark")


# RequestManagerBenchmark

add_executable(RequestManagerBenchmark EXCLUDE_FROM_ALL
    ${REQUEST_MANAGER_BENCHMARK_SOURCES_FOLDER_NAME}/BenchmarkCommonData.h
    ${REQUEST_MANAGER_BENCHMARK_SOURCES_FOLDER_NAME}/benchmark.cpp
)

target_include_directories(RequestManagerBenchmark
    PRIVATE
        ${SOURCES_DIR_PATH}
)

target_link_libraries(RequestManagerBenchmark
    PRIVATE
        Qt6::Core
        Qt6::Network
        Network
)


# BenchmarkAnalyzer

add_executable(BenchmarkAnalyzer EXCLUDE_FROM_ALL
    ${REQUEST_MANAGER_BENCHMARK_SOURCES_FOLDER_NAME}/BenchmarkCommonData.h
    ${REQUEST_MANAGER_BENCHMARK_SOURCES_FOLDER_NAME}/benchmarkAnalyzer.cpp
)

target_link_libraries(BenchmarkAnalyzer
    PRIVATE
        Qt6::Core
)