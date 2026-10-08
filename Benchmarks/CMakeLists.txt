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

set_target_properties(BenchmarkCommonData PROPERTIES FOLDER "RequestManagerBenchmark")



add_executable(RequestManagerBenchmark EXCLUDE_FROM_ALL
    RequestManagerBenchmark/benchmark.cpp
)

target_include_directories(RequestManagerBenchmark
    PRIVATE
        ${SOURCES_DIR_PATH}
)

target_link_libraries(RequestManagerBenchmark
    PRIVATE
        App #TODO: change to AppController
        NetworkManager
        LinkedInRequestStrategy
        RequestManager
        BenchmarkCommonData
        Qt6::Network
)

set_target_properties(RequestManagerBenchmark PROPERTIES FOLDER "RequestManagerBenchmark")



add_executable(BenchmarkAnalyzer EXCLUDE_FROM_ALL
    RequestManagerBenchmark/benchmarkAnalyzer.cpp
)

target_link_libraries(BenchmarkAnalyzer
    PRIVATE
        BenchmarkCommonData
)

set_target_properties(BenchmarkAnalyzer PROPERTIES FOLDER "RequestManagerBenchmark")