#pragma once

// std

// Qt
#include <QString>
#include <QDate>
#include <QTextStream>
#include <QLoggingCategory>

// custom

namespace Linked
{
    Q_DECLARE_LOGGING_CATEGORY(LinkedVacancy);

    class Vacancy
    {
    public:
        Vacancy() = default;
        ~Vacancy() = default;

    public:
        void print(QTextStream &) const;
        qsizetype count(const QString &str, Qt::CaseSensitivity cs = Qt::CaseInsensitive) const;
        void clear();

    public:
        QString title = "Not Given";
        QString link = "Not Given";
        QString companyName = "Not Given";
        QString companyLink = "Not Given";
        QString description = "Not Given";
        QString location = "Not Given";
        QString level = "Not Given";
        QString employmentType = "Not Given";
        QDate date;
    };

} // namespace Linked