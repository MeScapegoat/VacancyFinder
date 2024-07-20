#pragma once
#include <QString>
#include <QDate>
#include <QTextStream>

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