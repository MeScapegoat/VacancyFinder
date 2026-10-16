#include "VacancyBrowserPresenter.h"

#include <QListView>
#include <QStringListModel>

#include "VacancyDetails.h"


Q_LOGGING_CATEGORY(VBPLog, "vacancyBrowser.presenter");


VacancyBrowserPresenter::VacancyBrowserPresenter(QObject* parent) :
    Presenter(parent)
{
    VacancyBrowserView& view = getView();

    QObject::connect(&view.vacancyList(), &VacancyBrowserView::VacancyList::clicked, this, &VacancyBrowserPresenter::openVacancyDetails);

    QObject::connect(&view.vacancyDetails(), &VacancyDetails::ignoreButtonPressed,     this, &VacancyBrowserPresenter::onIgnoreButtonPressed);
    QObject::connect(&view.vacancyDetails(), &VacancyDetails::interestedButtonPressed, this, &VacancyBrowserPresenter::onInterestButtonPressed);

    QObject::connect(&view.vacancyDetails(), &VacancyDetails::removeIgnoreButtonPressed,     this, &VacancyBrowserPresenter::onRemoveIgnoreButtonPressed);
    QObject::connect(&view.vacancyDetails(), &VacancyDetails::removeInterestedButtonPressed, this, &VacancyBrowserPresenter::onRemoveInterestButtonPressed);

    getView().vacancyList().setModel(&getModel().vacancyListModel());

    getView().vacancyDetails().close();
}

const QSet<QUrl>& VacancyBrowserPresenter::ignoredVacancies() const
{
    return getModel().ignoredVacancies();
}

void VacancyBrowserPresenter::setIgnoredVacancies(const QSet<QUrl>& ignoredVacancies)
{
    getModel().setIgnoredVacancies(ignoredVacancies);
}

const QSet<QUrl>& VacancyBrowserPresenter::interestedVacancies() const
{
    return getModel().interestedVacancies();
}

void VacancyBrowserPresenter::setInterestedVacancies(const QSet<QUrl>& interestedVacancies)
{
    getModel().setInterestedVacancies(interestedVacancies);
}

void VacancyBrowserPresenter::reset()
{
    VacancyBrowserModel& model = getModel();

    model.vacancyByUrl().clear();
    model.visibleVacancies().clear();
}

void VacancyBrowserPresenter::onBriefReady(const Vacancy& brief)
{
    qCDebug(VBPLog) << "received brief of" << brief.title;

    getModel().addVacancy(brief);
}

void VacancyBrowserPresenter::onDetailsReady(const Vacancy& details)
{
    qCDebug(VBPLog) << "received details of" << details.title;

    getModel().addVacancy(details);
}

void VacancyBrowserPresenter::updateVisibleVacancies()
{
    getModel().updateVisibleVacancies();
}

void VacancyBrowserPresenter::setVisibleStatuses(QList<VacancyStatus> statuses)
{
    getModel().setVisibleStatuses(statuses);
}

void VacancyBrowserPresenter::setVacancyFilter(const VacancyFilter& filter)
{
    getModel().setVacancyFilter(filter);
}

void VacancyBrowserPresenter::setVacancySortComparator(const VacancySortComparator& sortComparator)
{
    getModel().setVacancySortComparator(sortComparator);
}

void VacancyBrowserPresenter::openVacancyDetails(const QModelIndex& index)
{
    const VacancyBrowserModel& model = getModel();

    const int id = index.row();
    auto vacancy = model.getVisibleVacancy(id);
    if (!vacancy.has_value())
    {
        return;
    }

    const VacancyStatus status = model.getVacancyStatus(vacancy.value());

    getView().vacancyDetails().open(vacancy.value(), status);
}

void VacancyBrowserPresenter::onIgnoreButtonPressed()
{
    QSet<QUrl>& ignoredVacancies = getModel().ignoredVacancies();
    const bool needUpdate = addCurrentVacancyToStatusList(ignoredVacancies);

    if (needUpdate)
    {
        updateStatusButtons(VacancyStatus::Ignored);
    }
}

void VacancyBrowserPresenter::onInterestButtonPressed()
{
    QSet<QUrl>& interestedVacancies = getModel().interestedVacancies();
    const bool needUpdate = addCurrentVacancyToStatusList(interestedVacancies);

    if (needUpdate)
    {
        updateStatusButtons(VacancyStatus::Interested);
    }
}

void VacancyBrowserPresenter::onRemoveIgnoreButtonPressed()
{
    QSet<QUrl>& ignoredVacancies = getModel().ignoredVacancies();
    const bool needUpdate = removeCurrentVacancyFromStatusList(ignoredVacancies);

    if (needUpdate)
    {
        updateStatusButtons(VacancyStatus::Unreviewed);
    }
}

void VacancyBrowserPresenter::onRemoveInterestButtonPressed()
{
    QSet<QUrl>& interestedVacancies = getModel().interestedVacancies();
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

    getView().vacancyDetails().updateCurrentStatusButton(status);

    getView().vacancyDetails().close();
}

std::optional<constVacancyRef> VacancyBrowserPresenter::getCurrentVacancy() const
{
    const QModelIndex& index = getView().vacancyList().currentIndex();
    const VacancyBrowserModel& model = getModel();

    if (!index.isValid())
        return std::nullopt;

    const int id = index.row();

    return model.getVisibleVacancy(id);
}