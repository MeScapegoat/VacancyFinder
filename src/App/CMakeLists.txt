add_library(App
    App.h
    App.cpp

    AppState.h
    AppState.cpp

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
        Parsing
        UI
        ResultViewOptions
        Network
        QueryParameters
        ResultViewOptions
)