#pragma once

#include <QAbstractListModel>
#include <QHash>
#include <QSet>
#include <QString>
#include <QUrl>
#include <QVariant>
#include <QVector>

#include <Vacancy.h>
#include <VacancyStatus.h>

class VacancyListModel final : public QAbstractListModel
{
public:
    enum Role
    {
        TitleRole = Qt::UserRole + 1,
        CompanyRole,
        LocationRole,
        AgeRole,
        StatusLabelRole,
        StatusRole
    };

    explicit VacancyListModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setVacancies(const QVector<const Vacancy*>& vacancies,
                      const QSet<QUrl>& ignoredVacancies,
                      const QSet<QUrl>& interestedVacancies);

private:
    struct Item
    {
        QString title;
        QString company;
        QString location;
        QString age;
        VacancyStatus status;
    };

    QVector<Item> m_items;
};
