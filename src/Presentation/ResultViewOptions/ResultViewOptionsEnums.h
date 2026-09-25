#pragma once

enum class SortMode
{
    Off = 0,
    Relevance,

    Title,
    Company,

    Time,

    EmploymentType,

    Level,
    Experience,

    Education,
    Skills,

    Industry,

    ExpirationDate,


    COUNT
};

enum class FilterMode
{
    Off = 0,

    Include,
    Exclude,


    COUNT
};

enum class VacancyStatus
{
    Unreviewed = 0,

    Interested,
    Ignored,


    COUNT
};