#pragma once

// std

// Qt
#include <QString>
#include <QDate>
#include <QTextStream>
#include <QLoggingCategory>

// custom

Q_DECLARE_LOGGING_CATEGORY(LinkedVacancy);

namespace Linked
{
    class Vacancy
    {
    public:
        Vacancy() = default;
        ~Vacancy() = default;
        Vacancy(const QString &);

    public:
        void print(QTextStream &) const;
        qsizetype count(const QString &str, Qt::CaseSensitivity cs = Qt::CaseInsensitive) const;
        void clear();

    public:
        QString title;
        QString link;
        QString companyName;
        QString companyLink;
        QString description;
        QDate date;
        QString location;
        QString level;
        QString employmentType;
    };

} // namespace Linked