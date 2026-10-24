add_library(ResultViewOptions
    ResultViewOptionsEnums.h

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
        FunctionTypes
        VacancyStatus
    PRIVATE
        CallableComboBox
        VacancyOperations
        Domain
)

set_target_properties(ResultViewOptions PROPERTIES FOLDER "src/Presentation")
