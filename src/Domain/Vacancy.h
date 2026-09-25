#pragma once

#include <optional>

#include <QLoggingCategory>
#include <QUrl>
#include <QString>
#include <QDate>
#include <QDebug>


Q_DECLARE_LOGGING_CATEGORY(VacancyLog);

using Months = size_t;

class Vacancy
{
public:
    Vacancy() = default;
    ~Vacancy() = default;

public:
    qsizetype count(const QString& str, Qt::CaseSensitivity cs = Qt::CaseInsensitive) const noexcept;

    void fillEmptyFrom(Vacancy other) noexcept;

public:
    size_t id = 0;

    QString title;
    QUrl link;

    QString companyName;
    QUrl companyLink;

    QString location;

    QString employmentType;

    QString skills;

    QString description;

    QDate date;


    std::optional<QString> industry;

    std::optional<QString> level;
    std::optional<Months> experience;

    std::optional<QString> education;

    std::optional<QDate> expirationDate;
};

QDebug operator<<(QDebug debug, const Vacancy& vacancy) noexcept;