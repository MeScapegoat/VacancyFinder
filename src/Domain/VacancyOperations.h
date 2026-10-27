#pragma once

#include <QStringList>

class Vacancy;


bool containsAnyFilter(const QStringList& filterWords, const Vacancy* vacancy);
bool containsAllFilter(const QStringList& filterWords, const Vacancy* vacancy);
bool excludesAnyFilter(const QStringList& filterWords, const Vacancy* vacancy);
bool excludesAllFilter(const QStringList& filterWords, const Vacancy* vacancy);

bool relevanceSort(const QString& keywords, const Vacancy* left, const Vacancy* right);
bool titleSort(const Vacancy* left, const Vacancy* right);
bool companySort(const Vacancy* left, const Vacancy* right);
bool timeSort(const Vacancy* left, const Vacancy* right);
bool employmentTypeSort(const Vacancy* left, const Vacancy* right);
bool levelSort(const Vacancy* left, const Vacancy* right);
bool experienceSort(const Vacancy* left, const Vacancy* right);
bool educationSort(const Vacancy* left, const Vacancy* right);
bool skillsSort(const Vacancy* left, const Vacancy* right);
bool industrySort(const Vacancy* left, const Vacancy* right);
bool expirationDateSort(const Vacancy* left, const Vacancy* right);
