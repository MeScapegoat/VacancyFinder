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
    #PRIVATE # TODO: retrieve
        Domain
        Parser
        LinkedInRequestStrategy
        $<$<CONFIG:Release>:LinkedInParserStrategy>
        $<$<CONFIG:Debug>:LinkedInPageCollector>
        UI
        ResultViewOptions
        RequestManager
        NetworkManager
        QueryParameters
        ResultViewOptions
        VacancyBrowser
    PRIVATE
        WorkCoordinator
        Utils
)
