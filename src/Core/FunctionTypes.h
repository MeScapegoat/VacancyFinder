#pragma once

#include <functional>

class Vacancy;

using VacancyFilter = std::function<bool(const Vacancy*)>;
using VacancySortComparator = std::function<bool(const Vacancy*, const Vacancy*)>;