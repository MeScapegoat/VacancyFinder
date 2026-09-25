add_library(ResultViewOptions
    ResultViewOptionsModel.h
    ResultViewOptionsModel.cpp

    ResultViewOptionsView.h
    ResultViewOptionsView.cpp

    ResultViewOptionsPresenter.h
    ResultViewOptionsPresenter.cpp
)

target_include_directories(ResultViewOptions PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(ResultViewOptions
    PUBLIC
        Presentation
        Utils
)