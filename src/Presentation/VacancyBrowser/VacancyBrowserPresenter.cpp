#include "VacancyBrowserPresenter.h"

#include <QItemSelectionModel>
#include <QModelIndex>

#include "VacancyDetails.h"

VacancyBrowserPresenter::VacancyBrowserPresenter(QObject* parent) :
    Presenter(parent)
{

    QObject::connect(&view().vacancyDetails(), &VacancyDetails::ignoreButtonPressed,     this, &VacancyBrowserPresenter::onIgnoreButtonPressed);
    QObject::connect(&view().vacancyDetails(), &VacancyDetails::interestedButtonPressed, this, &VacancyBrowserPresenter::onInterestButtonPressed);

    QObject::connect(&view().vacancyDetails(), &VacancyDetails::removeIgnoreButtonPressed,     this, &VacancyBrowserPresenter::onRemoveIgnoreButtonPressed);
    QObject::connect(&view().vacancyDetails(), &VacancyDetails::removeInterestedButtonPressed, this, &VacancyBrowserPresenter::onRemoveInterestButtonPressed);

    view().vacancyList().setModel(&model().vacancyListModel());
    QObject::connect(view().vacancyList().selectionModel(), &QItemSelectionModel::currentChanged, this, &VacancyBrowserPresenter::openVacancyDetails);

    view().vacancyDetails().close();
}

const QSet<QUrl>& VacancyBrowserPresenter::ignoredVacancies() const
{
    return model().ignoredVacancies();
}

void VacancyBrowserPresenter::setIgnoredVacancies(const QSet<QUrl>& ignoredVacancies)
{
    model().setIgnoredVacancies(ignoredVacancies);
}

const QSet<QUrl>& VacancyBrowserPresenter::interestedVacancies() const
{
    return model().interestedVacancies();
}

void VacancyBrowserPresenter::setInterestedVacancies(const QSet<QUrl>& interestedVacancies)
{
    model().setInterestedVacancies(interestedVacancies);
    emit interestedVacanciesChanged();
}

void VacancyBrowserPresenter::reset()
{
    m_currentVacancyUrl.reset();
    model().vacancyByUrl().clear();
    model().visibleVacancies().clear();

    updateVisibleVacancies();
    view().vacancyDetails().close();
}

void VacancyBrowserPresenter::onVacanciesReady(const QList<Vacancy>& vacancies)
{
    for (const Vacancy& vacancy : vacancies)
    {
        model().addVacancy(vacancy);
    }

    updateVisibleVacancies();
}

void VacancyBrowserPresenter::updateVisibleVacancies()
{
    model().updateVisibleVacancies();
}

void VacancyBrowserPresenter::setVisibleStatuses(QList<VacancyStatus> statuses)
{
    model().setVisibleStatuses(statuses);
}

void VacancyBrowserPresenter::setVacancyFilter(const VacancyFilter& filter)
{
    model().setVacancyFilter(filter);
}

void VacancyBrowserPresenter::setVacancySortComparator(const VacancySortComparator& sortComparator)
{
    model().setVacancySortComparator(sortComparator);
}

void VacancyBrowserPresenter::openVacancyDetails(const QModelIndex& index)
{
    const int id = index.row();
    auto vacancy = model().getVisibleVacancy(id);
    if (!vacancy.has_value())
    {
        m_currentVacancyUrl.reset();
        view().vacancyDetails().close();
        return;
    }

    m_currentVacancyUrl = vacancy->get().url;
    const VacancyStatus status = model().getVacancyStatus(vacancy.value());

    view().vacancyDetails().open(vacancy.value(), status);
}

void VacancyBrowserPresenter::onIgnoreButtonPressed()
{
    QSet<QUrl>& ignoredVacancies = model().ignoredVacancies();
    const bool needUpdate = addCurrentVacancyToStatusList(ignoredVacancies);

    if (needUpdate)
    {
        updateStatusButtons(VacancyStatus::Ignored);
    }
}

void VacancyBrowserPresenter::onInterestButtonPressed()
{
    QSet<QUrl>& interestedVacancies = model().interestedVacancies();
    const bool needUpdate = addCurrentVacancyToStatusList(interestedVacancies);

    if (needUpdate)
    {
        updateStatusButtons(VacancyStatus::Interested);
        emit interestedVacanciesChanged();
    }
}

void VacancyBrowserPresenter::onRemoveIgnoreButtonPressed()
{
    QSet<QUrl>& ignoredVacancies = model().ignoredVacancies();
    const bool needUpdate = removeCurrentVacancyFromStatusList(ignoredVacancies);

    if (needUpdate)
    {
        updateStatusButtons(VacancyStatus::Unreviewed);
    }
}

void VacancyBrowserPresenter::onRemoveInterestButtonPressed()
{
    QSet<QUrl>& interestedVacancies = model().interestedVacancies();
    const bool needUpdate = removeCurrentVacancyFromStatusList(interestedVacancies);

    if (needUpdate)
    {
        updateStatusButtons(VacancyStatus::Unreviewed);
        emit interestedVacanciesChanged();
    }
}

bool VacancyBrowserPresenter::addCurrentVacancyToStatusList(QSet<QUrl>& statusVacancyList)
{
    if (!m_currentVacancyUrl || !model().vacancyByUrl().contains(*m_currentVacancyUrl))
        return false;

    const qsizetype oldSize = statusVacancyList.size();

    statusVacancyList.insert(*m_currentVacancyUrl);

    return oldSize != statusVacancyList.size();
}

bool VacancyBrowserPresenter::removeCurrentVacancyFromStatusList(QSet<QUrl>& statusVacancyList)
{
    if (!m_currentVacancyUrl || !model().vacancyByUrl().contains(*m_currentVacancyUrl))
        return false;

    return statusVacancyList.remove(*m_currentVacancyUrl);
}

void VacancyBrowserPresenter::updateStatusButtons(VacancyStatus status)
{
    updateVisibleVacancies();

    view().vacancyDetails().updateCurrentStatusButton(status);

    m_currentVacancyUrl.reset();
    view().vacancyDetails().close();
}
