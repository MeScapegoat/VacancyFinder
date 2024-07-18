#pragma once
#include <QString>
#include <QDate>
#include <QTextStream>

class LinkedVacancy
{
public:
    LinkedVacancy() = default;
    ~LinkedVacancy() = default;
    LinkedVacancy(const QString &);

public:
    void load(QString);
    void loadDescription(QString);
    void print(QTextStream &) const;
    qsizetype count(const QString& str, Qt::CaseSensitivity cs = Qt::CaseInsensitive) const;
    void clear();

public:
    QString title;
    QString link;
    QString companyName;
    QString companyLink;
    QString description;
    QDate date;
    QString location;

private:
    QString extract(QString data, const QString &start, const QString &end);
};