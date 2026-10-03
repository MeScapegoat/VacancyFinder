#include "VacancyExtractor.h"

Vacancy VacancyExtractor::vacancy() const
{
    Vacancy result;

    result.title = title();
    result.link = link();
    result.companyName = companyName();
    result.companyLink = companyLink();
    result.location = location();
    result.employmentType = employmentType();
    result.skills = skills();
    result.description = description();
    result.date = date();
    result.industry = industry();
    result.level = level();
    result.experience = experience();
    result.education = education();
    result.expirationDate = expirationDate();

    return result;
}

QString VacancyExtractor::title() const
{
    return QString();
};

QUrl VacancyExtractor::link() const
{
    return QUrl();
};

QString VacancyExtractor::companyName() const
{
    return QString();
};

QUrl VacancyExtractor::companyLink() const
{
    return QUrl();
};

QString VacancyExtractor::location() const
{
    return QString();
};

QString VacancyExtractor::employmentType() const
{
    return QString();
};

QString VacancyExtractor::skills() const
{
    return QString();
};

QString VacancyExtractor::description() const
{
    return QString();
};

QDate VacancyExtractor::date() const
{
    return QDate();
};

std::optional<QString> VacancyExtractor::industry() const
{
    return std::nullopt;
};

std::optional<QString> VacancyExtractor::level() const
{
    return std::nullopt;
};

std::optional<Months> VacancyExtractor::experience() const
{
    return std::nullopt;
};

std::optional<QString> VacancyExtractor::education() const
{
    return std::nullopt;
};

std::optional<QDate> VacancyExtractor::expirationDate() const
{
    return std::nullopt;
};