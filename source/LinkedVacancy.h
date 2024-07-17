
#include <QString>
#include <QTextStream>

class LinkedVacancy
{
public:
    LinkedVacancy() = default;
    ~LinkedVacancy() = default;
    LinkedVacancy(const QString &);

public:
    void load(QString);
    void print(QTextStream &);

private:
    QString extract(QString data, const QString &start, const QString &end);
    QString loadDescription(const QString &);

private:
    QString title;
    QString link;
    QString companyName;
    QString companyLink;
    QString description;
    QString publishTime;
    QString location;
};