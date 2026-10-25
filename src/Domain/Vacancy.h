#pragma once

#include <QDate>
#include <QDebug>
#include <QString>
#include <QUrl>

#include <optional>


using Months = size_t;

class Vacancy
{
public:
    Vacancy() = default;
    ~Vacancy() = default;

public:
    qsizetype count(const QString& str, Qt::CaseSensitivity cs = Qt::CaseInsensitive) const noexcept;
    bool contains(const QString& str, Qt::CaseSensitivity cs = Qt::CaseSensitive) const noexcept;

    void fillEmptyFrom(Vacancy other) noexcept;

public:
    size_t id = 0;

    QString title;
    QUrl url;

    QString companyName;
    QUrl companyUrl;

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
