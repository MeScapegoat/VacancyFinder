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
        Domain
        AbstractSearchUrlProvider
    PRIVATE
        AbstractParserStrategy
        AbstractRequestStrategy
        Parser
        RequestManager
        WorkCoordinator
)

#set_target_properties(VacancySearchWorkflow PROPERTIES FOLDER "App")



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
        $<$<CONFIG:Release>:LinkedInParserStrategy>
        $<$<CONFIG:Debug>:LinkedInPageCollector>
)

#set_target_properties(LinkedInVacancySearchWorkflow PROPERTIES FOLDER "App")



add_library(App
    App.h
    App.cpp

    AppState.h
    AppState.cpp
)

target_include_directories(App PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(App
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
