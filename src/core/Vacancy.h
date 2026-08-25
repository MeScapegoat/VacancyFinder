#pragma once

// std

// Qt
#include <QString>
#include <QDate>
#include <QTextStream>
#include <QLoggingCategory>
#include <QUrl>

// custom

namespace Linked
{
    Q_DECLARE_LOGGING_CATEGORY(LinkedVacancy);

    using Months = unsigned;

    class Vacancy
    {
    public:
        Vacancy();
        ~Vacancy() = default;

    public:
        qsizetype count(const QString &str, Qt::CaseSensitivity cs = Qt::CaseInsensitive) const;
        void clear();

        void mergeFrom(const Vacancy& vacancy);

        // make many variables optional since they might be missing
    public:
        QString title;
        QUrl link;

        QString companyName;
        QString industry;
        QUrl companyLink;

        QString location;
        QDate date;
        QDate expirationDate;

        Months experience;
        QString education;
        QString skills;

        QString level;
        QString employmentType;

        QString description;
    };

    QDebug operator<<(QDebug debug, const Vacancy& vacancy);

} // namespace Linked