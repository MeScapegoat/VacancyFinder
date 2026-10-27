#include "VacancyListModel.h"

#include <QDate>
#include <QString>
#include <QVariant>

#include <utility>

VacancyListModel::VacancyListModel(QObject* parent) :
    QAbstractListModel(parent)
{
}

int VacancyListModel::rowCount(const QModelIndex& parent) const
{
    return parent.isValid() ? 0 : static_cast<int>(m_items.size());
}

QVariant VacancyListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.column() != 0 || index.row() < 0 || index.row() >= static_cast<int>(m_items.size()))
    {
        return {};
    }

    const Item& item = m_items[index.row()];
    switch (role)
    {
    case Qt::DisplayRole:
        return QString("%1 — %2 · %3 · %4").arg(item.title, item.company, item.location, item.age);
    case TitleRole:
        return item.title;
    case CompanyRole:
        return item.company;
    case LocationRole:
        return item.location;
    case AgeRole:
        return item.age;
    case StatusLabelRole:
        switch (item.status)
        {
        case VacancyStatus::Interested:
            return QStringLiteral("Interested");
        case VacancyStatus::Ignored:
            return QStringLiteral("Not interested");
        case VacancyStatus::Unreviewed:
            return QStringLiteral("New");
        case VacancyStatus::COUNT:
            break;
        }
        return QStringLiteral("New");
    case StatusRole:
        return static_cast<int>(item.status);
    default:
        return {};
    }
}

QHash<int, QByteArray> VacancyListModel::roleNames() const
{
    return {
        {TitleRole, "title"},
        {CompanyRole, "company"},
        {LocationRole, "location"},
        {AgeRole, "age"},
        {StatusLabelRole, "statusLabel"},
        {StatusRole, "status"}
    };
}

void VacancyListModel::setVacancies(const QVector<const Vacancy*>& vacancies,
                                    const QSet<QUrl>& ignoredVacancies,
                                    const QSet<QUrl>& interestedVacancies)
{
    beginResetModel();
    m_items.clear();
    m_items.reserve(vacancies.size());

    for (const Vacancy* vacancy : vacancies)
    {
        const qint64 ageInDays = vacancy->date.isValid() ? vacancy->date.daysTo(QDate::currentDate()) : 0;
        QString age;
        if (!vacancy->date.isValid())
        {
            age = QStringLiteral("Date unavailable");
        }
        else if (ageInDays == 0)
        {
            age = QStringLiteral("Today");
        }
        else if (ageInDays == 1)
        {
            age = QStringLiteral("Yesterday");
        }
        else if (ageInDays > 1)
        {
            age = QStringLiteral("%1 days ago").arg(ageInDays);
        }
        else
        {
            age = QStringLiteral("In %1 days").arg(-ageInDays);
        }

        const VacancyStatus status = interestedVacancies.contains(vacancy->url)
            ? VacancyStatus::Interested
            : ignoredVacancies.contains(vacancy->url) ? VacancyStatus::Ignored : VacancyStatus::Unreviewed;

        m_items.push_back({
            vacancy->title.isEmpty() ? QStringLiteral("Untitled vacancy") : vacancy->title,
            vacancy->companyName.isEmpty() ? QStringLiteral("Company not provided") : vacancy->companyName,
            vacancy->location.isEmpty() ? QStringLiteral("Location not provided") : vacancy->location,
            std::move(age),
            status
        });
    }

    endResetModel();
}
