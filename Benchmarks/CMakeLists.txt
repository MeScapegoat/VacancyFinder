# RequestManagerBenchmark

add_executable(RequestManagerBenchmark EXCLUDE_FROM_ALL
    RequestManagerBenchmark/BenchmarkCommonData.h
    RequestManagerBenchmark/benchmark.cpp
)

target_include_directories(RequestManagerBenchmark
    PRIVATE
        ${SOURCES_DIR_PATH}
)

target_link_libraries(RequestManagerBenchmark
    PRIVATE
        App
        Network
        Qt6::Core
        Qt6::Network
)


# BenchmarkAnalyzer

add_executable(BenchmarkAnalyzer EXCLUDE_FROM_ALL
    RequestManagerBenchmark/BenchmarkCommonData.h
    RequestManagerBenchmark/benchmarkAnalyzer.cpp
)

target_link_libraries(BenchmarkAnalyzer
    PRIVATE
        Qt6::Core
)