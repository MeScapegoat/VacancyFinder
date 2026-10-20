#include "VacancyBrowserPresenter.h"

#include <QListView>
#include <QStringListModel>

#include "VacancyDetails.h"


Q_LOGGING_CATEGORY(VBPLog, "vacancyBrowser.presenter");


VacancyBrowserPresenter::VacancyBrowserPresenter(QObject* parent) :
    Presenter(parent)
{

    QObject::connect(&view().vacancyList(), &VacancyBrowserView::VacancyList::clicked, this, &VacancyBrowserPresenter::openVacancyDetails);

    QObject::connect(&view().vacancyDetails(), &VacancyDetails::ignoreButtonPressed,     this, &VacancyBrowserPresenter::onIgnoreButtonPressed);
    QObject::connect(&view().vacancyDetails(), &VacancyDetails::interestedButtonPressed, this, &VacancyBrowserPresenter::onInterestButtonPressed);

    QObject::connect(&view().vacancyDetails(), &VacancyDetails::removeIgnoreButtonPressed,     this, &VacancyBrowserPresenter::onRemoveIgnoreButtonPressed);
    QObject::connect(&view().vacancyDetails(), &VacancyDetails::removeInterestedButtonPressed, this, &VacancyBrowserPresenter::onRemoveInterestButtonPressed);

    view().vacancyList().setModel(&model().vacancyListModel());

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
}

void VacancyBrowserPresenter::reset()
{
    model().vacancyByUrl().clear();
    model().visibleVacancies().clear();
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
        return;
    }

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
    }
}

bool VacancyBrowserPresenter::addCurrentVacancyToStatusList(QSet<QUrl>& statusVacancyList)
{
    const std::optional<constVacancyRef> vacancy = getCurrentVacancy();
    if (!vacancy)
        return false;

    const qsizetype oldSize = statusVacancyList.size();

    statusVacancyList.insert(vacancy->get().link);

    return oldSize != statusVacancyList.size();
}

bool VacancyBrowserPresenter::removeCurrentVacancyFromStatusList(QSet<QUrl>& statusVacancyList)
{
    const std::optional<constVacancyRef> vacancy = getCurrentVacancy();
    if (!vacancy)
        return false;

    return statusVacancyList.remove(vacancy->get().link);
}

void VacancyBrowserPresenter::updateStatusButtons(VacancyStatus status)
{
    updateVisibleVacancies();

    view().vacancyDetails().updateCurrentStatusButton(status);

    view().vacancyDetails().close();
}

std::optional<constVacancyRef> VacancyBrowserPresenter::getCurrentVacancy() const
{
    const QModelIndex& index = view().vacancyList().currentIndex();

    if (!index.isValid())
        return std::nullopt;

    const int id = index.row();

    return model().getVisibleVacancy(id);
}
