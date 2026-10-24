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

set_target_properties(Domain PROPERTIES FOLDER "src/Domain")



add_library(VacancyOperations
    VacancyOperations.h
    VacancyOperations.cpp
)

target_include_directories(VacancyOperations PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(VacancyOperations
    PRIVATE
        Domain
)

set_target_properties(VacancyOperations PROPERTIES FOLDER "src/Domain")



add_library(VacancyStatus
    VacancyStatus.h
)

target_include_directories(VacancyStatus INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

set_target_properties(VacancyStatus PROPERTIES FOLDER "src/Domain")
