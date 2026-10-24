add_library(VacancyBrowser
    VacancyBrowserModel.h
    VacancyBrowserModel.cpp

    VacancyBrowserView.h
    VacancyBrowserView.cpp

    VacancyBrowserPresenter.h
    VacancyBrowserPresenter.cpp

    VacancyDetails.h
    VacancyDetails.cpp
)

target_include_directories(VacancyBrowser PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(VacancyBrowser
    PUBLIC
        Presentation
        Domain
        FunctionTypes
        VacancyStatus
)

set_target_properties(VacancyBrowser PROPERTIES FOLDER "src/Presentation")
