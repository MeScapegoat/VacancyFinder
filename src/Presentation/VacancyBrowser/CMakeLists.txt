add_library(VacancyBrowser
    VacancyBrowserEnums.h

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
    PRIVATE
        Domain
)