add_library(BenchmarkCommonData EXCLUDE_FROM_ALL
    RequestManagerBenchmark/BenchmarkCommonData.h)

target_include_directories(BenchmarkCommonData
    INTERFACE
        ${SOURCES_DIR_PATH}
)

target_link_libraries(BenchmarkCommonData
    INTERFACE
        Qt6::Core
)

set_target_properties(BenchmarkCommonData PROPERTIES FOLDER "Benchmarks/RequestManagerBenchmark")



add_executable(LinkedInRequestManagerBenchmark EXCLUDE_FROM_ALL
    RequestManagerBenchmark/LinkedInRequestManagerBenchmark.cpp
)

target_include_directories(LinkedInRequestManagerBenchmark
    PRIVATE
        ${SOURCES_DIR_PATH}
)

target_link_libraries(LinkedInRequestManagerBenchmark
    PRIVATE
        WorkCoordinator
        NetworkManager
        LinkedInRequestStrategy
        RequestManager
        BenchmarkCommonData
        Qt6::Network
)

set_target_properties(LinkedInRequestManagerBenchmark PROPERTIES FOLDER "Benchmarks/RequestManagerBenchmark")



add_executable(BenchmarkAnalyzer EXCLUDE_FROM_ALL
    RequestManagerBenchmark/benchmarkAnalyzer.cpp
)

target_link_libraries(BenchmarkAnalyzer
    PRIVATE
        BenchmarkCommonData
)

set_target_properties(BenchmarkAnalyzer PROPERTIES FOLDER "Benchmarks/RequestManagerBenchmark")
