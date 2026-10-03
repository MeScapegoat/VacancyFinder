#pragma once

#include "Vacancy.h"

class VacancyExtractor
{
public:
    VacancyExtractor() = default;
    virtual ~VacancyExtractor() = default;

public:
    Vacancy vacancy() const;

protected:
    virtual QString title() const;
    virtual QUrl link() const;
    virtual QString companyName() const;
    virtual QUrl companyLink() const;
    virtual QString location() const;
    virtual QString employmentType() const;
    virtual QString skills() const;
    virtual QString description() const;
    virtual QDate date() const;
    virtual std::optional<QString> industry() const;
    virtual std::optional<QString> level() const;
    virtual std::optional<Months> experience() const;
    virtual std::optional<QString> education() const;
    virtual std::optional<QDate> expirationDate() const;
};