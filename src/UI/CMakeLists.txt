add_library(UI
    MainWindow.h
    MainWindow.cpp

    ProgressDialog.h
    ProgressDialog.cpp
)

target_include_directories(UI PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(UI
    PUBLIC 
        Qt6::Widgets
    #INTERFACE
        Qt6::Network
    PRIVATE
        Domain
        ResultViewOptions
            
)

set_target_properties(UI PROPERTIES FOLDER "src/UI")
