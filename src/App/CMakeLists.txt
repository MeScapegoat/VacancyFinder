add_library(App
    App.h
    App.cpp

    AppState.h
    AppState.cpp

    # TODO: make separate class
    AppController.h
    AppController.cpp
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
)