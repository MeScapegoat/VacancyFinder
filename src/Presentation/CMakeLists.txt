add_library(Presentation
    Model.h
    Model.cpp #TODO: remove
    View.h
    Presenter.hpp  
)

target_include_directories(Presentation PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Presentation
    PUBLIC
        Qt::Core
        Qt::Widgets
)

add_subdirectory(QueryParameters)
add_subdirectory(ResultViewOptions)