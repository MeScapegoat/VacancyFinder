add_library(WorkCoordinator
    WorkCoordinator.h
    WorkCoordinator.cpp
)

target_include_directories(WorkCoordinator PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(WorkCoordinator
    PUBLIC
        Qt6::Core
)

set_target_properties(WorkCoordinator PROPERTIES FOLDER "src/App")



add_library(VacancySearchWorkflow
    VacancySearchWorkflow.h
    VacancySearchWorkflow.cpp
)

target_include_directories(VacancySearchWorkflow PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(VacancySearchWorkflow
    PUBLIC
        Qt6::Core
    PRIVATE
        Domain
        AbstractSearchUrlProvider
        Parser
        RequestManager
        WorkCoordinator
)

set_target_properties(VacancySearchWorkflow PROPERTIES FOLDER "src/App")




add_library(LinkedInVacancySearchWorkflow
    LinkedInVacancySearchWorkflow.h
    LinkedInVacancySearchWorkflow.cpp
)

target_include_directories(LinkedInVacancySearchWorkflow PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(LinkedInVacancySearchWorkflow
    PUBLIC
        VacancySearchWorkflow
    PRIVATE
        AbstractParserStrategy
        LinkedInRequestStrategy
        LinkedInSearchUrlProvider
        NetworkManager
        Parser
        RequestManager
        $<$<CONFIG:Release>:LinkedInParserStrategy>
        $<$<CONFIG:Debug>:LinkedInPageCollector>
)

set_target_properties(LinkedInVacancySearchWorkflow PROPERTIES FOLDER "src/App")




add_library(VacancyFinder
    VacancyFinder.h
    VacancyFinder.cpp

    AppState.h
    AppState.cpp
)

target_include_directories(VacancyFinder PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(VacancyFinder
    PUBLIC 
        Qt6::Core
        Domain
        VacancySearchWorkflow
        UI
        ResultViewOptions
        QueryParameters
        ResultViewOptions
        VacancyBrowser
    PRIVATE
        LinkedInVacancySearchWorkflow
        Utils
)

set_target_properties(VacancyFinder PROPERTIES FOLDER "src/App")
