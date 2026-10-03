add_library(Domain
    Vacancy.h
    Vacancy.cpp

    VacancyExtractor.h
    VacancyExtractor.cpp
)

target_include_directories(Domain PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Domain
    PUBLIC 
        Qt6::Core
    PRIVATE
        Utils
)