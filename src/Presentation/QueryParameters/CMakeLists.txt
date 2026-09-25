add_library(QueryParameters
    QueryParametersModel.h
    QueryParametersModel.cpp

    QueryParametersView.h
    QueryParametersView.cpp

    QueryParametersPresenter.h
    QueryParametersPresenter.cpp
)

target_include_directories(QueryParameters PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(QueryParameters
    PUBLIC
        Presentation
        Utils
)