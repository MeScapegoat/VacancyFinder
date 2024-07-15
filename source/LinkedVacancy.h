
#include <QString>
#include <QTextStream>

class LinkedVacancy
{
public:
    LinkedVacancy() = default;
    ~LinkedVacancy() = default;
    LinkedVacancy(const QString &);

public:
    bool load(QString);
    void print(QTextStream &);
private:
    QString extract(QString data, const QString& start, const QString& end);

private:
    QString header;
    QString link;
    QString companyName;
    QString companyLink;
    QString description;
    QString publishTime;
    QString country;
    QString city;
};