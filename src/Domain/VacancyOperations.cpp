#include "VacancyOperations.h"

#include <Vacancy.h>
#include <QStringList>
#include <QString>

bool offFilter()
{
    return true;
}

bool containsAnyFilter(const QStringList& filterWords, const Vacancy* vacancy)
{
    if (filterWords.isEmpty())
        return true;

    for (const QString& word : filterWords)
    {
        if (vacancy->contains(word))
            return true;
    }

    return false;
}

bool containsAllFilter(const QStringList& filterWords, const Vacancy* vacancy)
{
    for (const QString& word : filterWords)
    {
        if (!vacancy->contains(word))
            return false;
    }

    return true;
}

bool excludesAnyFilter(const QStringList& filterWords, const Vacancy* vacancy)
{
    for (const QString& word : filterWords)
    {
        if (vacancy->contains(word))
            return false;
    }

    return true;
}

bool excludesAllFilter(const QStringList& filterWords, const Vacancy* vacancy)
{
    if (filterWords.isEmpty())
        return true;

    for (const QString& word : filterWords)
    {
        if (!vacancy->contains(word))
            return true;
    }

    return false;
}

bool relevanceSort(const QString& keywords, const Vacancy* left, const Vacancy* right)
{
    return left->count(keywords) > right->count(keywords);
}

bool titleSort(const Vacancy* left, const Vacancy* right)
{
    return left->title < right->title;
}

bool companySort(const Vacancy* left, const Vacancy* right)
{
    return left->companyName < right->companyName;
}

bool timeSort(const Vacancy* left, const Vacancy* right)
{
    return left->date > right->date;
}

bool employmentTypeSort(const Vacancy* left, const Vacancy* right)
{
    return left->employmentType > right->employmentType;
}

bool levelSort(const Vacancy* left, const Vacancy* right)
{
    if (!left->level.has_value()) return false;
    if (!right->level.has_value()) return true;

    return left->level < right->level;
}

bool experienceSort(const Vacancy* left, const Vacancy* right)
{
    if (!left->experience.has_value()) return false;
    if (!right->experience.has_value()) return true;

    return left->experience < right->experience;
}

bool educationSort(const Vacancy* left, const Vacancy* right)
{
    if (!left->education.has_value()) return false;
    if (!right->education.has_value()) return true;

    return left->education < right->education;
}

bool skillsSort(const Vacancy* left, const Vacancy* right)
{
    if (left->skills.isEmpty()) return false;
    if (right->skills.isEmpty()) return true;

    return left->skills < right->skills;
}

bool industrySort(const Vacancy* left, const Vacancy* right)
{
    if (!left->industry.has_value()) return false;
    if (!right->industry.has_value()) return true;

    return left->industry < right->industry;
}

bool expirationDateSort(const Vacancy* left, const Vacancy* right)
{
    if (!left->expirationDate.has_value()) return false;
    if (!right->expirationDate.has_value()) return true;

    return left->expirationDate < right->expirationDate;
}
