#pragma once
#include <optional>

#include <QDate>
#include <QDebug>
#include <QLoggingCategory>
#include <QString>
#include <QUrl>

Q_DECLARE_LOGGING_CATEGORY(VacancyLog);

using Months = unsigned;

// TODO: split it into title and description classes

class Vacancy
{
public:
    Vacancy();
    ~Vacancy();

public:
    qsizetype count(const QString& str, Qt::CaseSensitivity cs = Qt::CaseInsensitive) const;
    void reset();

    void mergeFrom(const Vacancy& vacancy);

public:
    unsigned id;

    QUrl link;

    std::optional<QString> title;

    std::optional<QString> companyName;
    std::optional<QString> industry;
    std::optional<QUrl> companyLink;

    std::optional<QString> location;
    std::optional<QDate> date;
    std::optional<QDate> expirationDate;

    std::optional<Months> experience;
    std::optional<QString> education;
    std::optional<QString> skills;

    std::optional<QString> level;
    std::optional<QString> employmentType;

    std::optional<QString> description;
};

QDebug operator<<(QDebug debug, const Vacancy& vacancy);

template <typename T>
void printVacancyItem(QDebug& debug, const char* name, const T& value)
{
    debug << qSetFieldWidth(16) << name << qSetFieldWidth(0) << ": " << value << '\n';
}

template <typename T>
void printVacancyItem(QDebug& debug, const char* name, const std::optional<T>& opt)
{
    if (opt)
        printVacancyItem(debug, name, opt.value());
}

