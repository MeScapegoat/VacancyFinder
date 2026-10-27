#include "ResultViewOptionsPresenter.h"

#include <QCheckBox>
#include <QLineEdit>
#include <QStringList>

#include <CallableComboBox.hpp>
#include <ResultViewOptionsEnums.h>
#include <Vacancy.h>
#include <VacancyOperations.h>

namespace
{
    int normalizeFilterMode(const int filterMode)
    {
        if (filterMode < static_cast<int>(FilterMode::Off) || filterMode >= static_cast<int>(FilterMode::COUNT))
        {
            return static_cast<int>(FilterMode::Off);
        }

        return filterMode;
    }

    int normalizeSortMode(const int sortMode)
    {
        if (sortMode < static_cast<int>(SortMode::Off) || sortMode >= static_cast<int>(SortMode::COUNT))
        {
            return static_cast<int>(SortMode::Off);
        }

        return sortMode;
    }
}

ResultViewOptionsPresenter::ResultViewOptionsPresenter(QObject* parent) : 
    Presenter(parent)
{
    insertFilters();
    insertSorts();


    connect(&view().filterMode(), &QComboBox::currentIndexChanged, this, &ResultViewOptionsPresenter::setFilterMode);
    connect(&model().currentFilterMode, &Observable<int>::valueChanged, this, &ResultViewOptionsPresenter::onCurrentFilterIndexChanged);
    onCurrentFilterIndexChanged();


    connect(&view().sortMode(), &QComboBox::currentIndexChanged, this, &ResultViewOptionsPresenter::setSortMode);
    connect(&model().currentSortMode, &Observable<int>::valueChanged, this, &ResultViewOptionsPresenter::onCurrentSortIndexChanged);
    onCurrentSortIndexChanged();


    connect(&view().filterLineEdit(), &QLineEdit::textChanged, this, &ResultViewOptionsPresenter::onFilterWordsWidgetChanged);
    connect(&model().filterWordList, &Observable<QStringList>::valueChanged, this, &ResultViewOptionsPresenter::onFilterWordsChanged);
    onFilterWordsWidgetChanged();
    onFilterWordsChanged();


    connect(&view().unreviewedCheckBox(), &QCheckBox::checkStateChanged, &model().isUnreviewedVisible, &Observable<bool>::setValue);
    connect(&model().isUnreviewedVisible, &Observable<bool>::valueChanged, this, &ResultViewOptionsPresenter::onUnreviewedVisibleChanged);
    onUnreviewedVisibleChanged();


    connect(&view().ignoredCheckBox(), &QCheckBox::checkStateChanged, &model().isIgnoredVisible, &Observable<bool>::setValue);
    connect(&model().isIgnoredVisible, &Observable<bool>::valueChanged, this, &ResultViewOptionsPresenter::onIgnoredVisibleChanged);
    onIgnoredVisibleChanged();


    connect(&view().interestedCheckBox(), &QCheckBox::checkStateChanged, &model().isInterestedVisible, &Observable<bool>::setValue);
    connect(&model().isInterestedVisible, &Observable<bool>::valueChanged, this, &ResultViewOptionsPresenter::onInterestedVisibleChanged);
    onInterestedVisibleChanged();


    QObject::connect(&model().currentFilterMode,   &AbstractObservable::valueChanged, this, &ResultViewOptionsPresenter::resultViewOptionsChanged);
    QObject::connect(&model().filterWordList,      &AbstractObservable::valueChanged, this, &ResultViewOptionsPresenter::resultViewOptionsChanged);
    QObject::connect(&model().currentSortMode,     &AbstractObservable::valueChanged, this, &ResultViewOptionsPresenter::resultViewOptionsChanged);
    QObject::connect(&model().isUnreviewedVisible, &AbstractObservable::valueChanged, this, &ResultViewOptionsPresenter::resultViewOptionsChanged);
    QObject::connect(&model().isIgnoredVisible,    &AbstractObservable::valueChanged, this, &ResultViewOptionsPresenter::resultViewOptionsChanged);
    QObject::connect(&model().isInterestedVisible, &AbstractObservable::valueChanged, this, &ResultViewOptionsPresenter::resultViewOptionsChanged);
}

void ResultViewOptionsPresenter::setFilterMode(int filterMode)
{
    const int normalizedFilterMode = normalizeFilterMode(filterMode);
    model().currentFilterMode = normalizedFilterMode;
}

int ResultViewOptionsPresenter::filterMode() const
{
    return model().currentFilterMode();
}

const VacancyFilter& ResultViewOptionsPresenter::vacancyFilter() const
{
    return view().filterMode().currentFunction();
}

void ResultViewOptionsPresenter::setFilterWords(const QString& filterWords)
{
    model().filterWordList = filterWordsToList(filterWords);
}

QString ResultViewOptionsPresenter::filterWords() const
{
    return filterWordsToString(model().filterWordList());
}

void ResultViewOptionsPresenter::setSortMode(int sortMode)
{
    const int normalizedSortMode = normalizeSortMode(sortMode);
    model().currentSortMode = normalizedSortMode;
}

int ResultViewOptionsPresenter::sortMode() const
{
    return model().currentSortMode();
}

const VacancySortComparator& ResultViewOptionsPresenter::vacancySortComparator() const
{
    return view().sortMode().currentFunction();
}

void ResultViewOptionsPresenter::setIsUnreviewedVisible(bool isVisible)
{
    model().isUnreviewedVisible = isVisible;
}

bool ResultViewOptionsPresenter::isUnreviewedVisible() const
{
    return model().isUnreviewedVisible();
}

void ResultViewOptionsPresenter::setIsIgnoredVisible(bool isVisible)
{
    model().isIgnoredVisible = isVisible;
}

bool ResultViewOptionsPresenter::isIgnoredVisible() const
{
    return model().isIgnoredVisible();
}

void ResultViewOptionsPresenter::setIsInterestedVisible(bool isVisible)
{
    model().isInterestedVisible = isVisible;
}

bool ResultViewOptionsPresenter::isInterestedVisible() const
{
    return model().isInterestedVisible();
}

QList<VacancyStatus> ResultViewOptionsPresenter::visibleStatuses() const
{
    QList<VacancyStatus> statuses;
    statuses.reserve(static_cast<int>(VacancyStatus::COUNT));

    if (model().isUnreviewedVisible()) statuses.push_back(VacancyStatus::Unreviewed);
    if (model().isIgnoredVisible()) statuses.push_back(VacancyStatus::Ignored);
    if (model().isInterestedVisible()) statuses.push_back(VacancyStatus::Interested);

    return statuses;
}

void ResultViewOptionsPresenter::setKeywords(const QString& keywords)
{
    model().setKeywords(keywords);
}

void ResultViewOptionsPresenter::onCurrentSortIndexChanged()
{
    const int normalizedSortMode = normalizeSortMode(model().currentSortMode());
    view().setSortMode(normalizedSortMode);

    emit vacancySortComparatorChanged(vacancySortComparator());
}

void ResultViewOptionsPresenter::onCurrentFilterIndexChanged()
{
    const int normalizedFilterMode = normalizeFilterMode(model().currentFilterMode());
    view().setFilterMode(normalizedFilterMode);

    emit vacancyFilterChanged(vacancyFilter());
}

void ResultViewOptionsPresenter::onFilterWordsChanged()
{
    view().setFilterLineEdit(filterWords());
}

void ResultViewOptionsPresenter::onFilterWordsWidgetChanged()
{
    const QString& filterWordsString = view().filterLineEdit().text();

    model().filterWordList = filterWordsToList(filterWordsString);
}

void ResultViewOptionsPresenter::onUnreviewedVisibleChanged()
{
    view().setUnreviewedCheckBox(model().isUnreviewedVisible());

    updateVisibleStatuses();
}

void ResultViewOptionsPresenter::onIgnoredVisibleChanged()
{
    view().setIgnoredCheckBox(model().isIgnoredVisible());

    updateVisibleStatuses();
}

void ResultViewOptionsPresenter::onInterestedVisibleChanged()
{
    view().setInterestedCheckBox(model().isInterestedVisible());

    updateVisibleStatuses();
}

void ResultViewOptionsPresenter::insertFilters()
{
    auto& filterMode = view().filterMode();

    VacancyFilter offFilterCallback;
    filterMode.insertItem(static_cast<qsizetype>(FilterMode::Off), QObject::tr("Off"), std::move(offFilterCallback));


    VacancyFilter containsAnyFilterCallback = [&filterWords = model().filterWordList()](const Vacancy* vacancy) -> bool
    {
        return containsAnyFilter(filterWords, vacancy);
    };
    filterMode.insertItem(static_cast<qsizetype>(FilterMode::ContainsAny), QObject::tr("Contains Any"), std::move(containsAnyFilterCallback));


    VacancyFilter containsAllFilterCallback = [&filterWords = model().filterWordList()](const Vacancy* vacancy) -> bool
    {
        return containsAllFilter(filterWords, vacancy);
    };
    filterMode.insertItem(static_cast<qsizetype>(FilterMode::ContainsAll), QObject::tr("Contains All"), std::move(containsAllFilterCallback));


    VacancyFilter excludesAnyFilterCallback = [&filterWords = model().filterWordList()](const Vacancy* vacancy) -> bool
    {
        return excludesAnyFilter(filterWords, vacancy);
    };
    filterMode.insertItem(static_cast<qsizetype>(FilterMode::ExcludesAny), QObject::tr("Excludes Any"), std::move(excludesAnyFilterCallback));


    VacancyFilter excludesAllFilterCallback = [&filterWords = model().filterWordList()](const Vacancy* vacancy) -> bool
    {
        return excludesAllFilter(filterWords, vacancy);
    };
    filterMode.insertItem(static_cast<qsizetype>(FilterMode::ExcludesAll), QObject::tr("Excludes All"), std::move(excludesAllFilterCallback));
}

void ResultViewOptionsPresenter::insertSorts()
{
    auto& sortMode = view().sortMode();

    VacancySortComparator offSortCallback;
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Off), tr("Off"), std::move(offSortCallback));

    VacancySortComparator relevanceSortCallback = [&keywords = model().keywords()](const Vacancy* left, const Vacancy* right) -> bool
    {
        return relevanceSort(keywords, left, right);
    };
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Relevance), tr("Relevance"), std::move(relevanceSortCallback));


    VacancySortComparator titleSortCallback = [](const Vacancy* left, const Vacancy* right) -> bool
    {
        return titleSort(left, right);
    };
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Title), tr("Title"), std::move(titleSortCallback));


    VacancySortComparator companySortCallback = [](const Vacancy* left, const Vacancy* right) -> bool
    {
        return companySort(left, right);
    };
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Company), tr("Company"), std::move(companySortCallback));


    VacancySortComparator timeSortCallback = [](const Vacancy* left, const Vacancy* right) -> bool
    {
        return timeSort(left, right);
    };
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Time), tr("Time"), std::move(timeSortCallback));


    VacancySortComparator employmentTypeSortCallback = [](const Vacancy* left, const Vacancy* right) -> bool
    {
        return employmentTypeSort(left, right);
    };
    sortMode.insertItem(static_cast<qsizetype>(SortMode::EmploymentType), tr("Employment Type"), std::move(employmentTypeSortCallback));


    VacancySortComparator levelSortCallback = [](const Vacancy* left, const Vacancy* right) -> bool
    {
        return levelSort(left, right);
    };
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Level), tr("Level"), std::move(levelSortCallback));


    VacancySortComparator experienceSortCallback = [](const Vacancy* left, const Vacancy* right) -> bool
    {
        return experienceSort(left, right);
    };
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Experience), tr("Experience"), std::move(experienceSortCallback));


    VacancySortComparator educationSortCallback = [](const Vacancy* left, const Vacancy* right) -> bool
    {
        return educationSort(left, right);
    };
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Education), tr("Education"), std::move(educationSortCallback));


    VacancySortComparator skillsSortCallback = [](const Vacancy* left, const Vacancy* right) -> bool
    {
        return skillsSort(left, right);
    };
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Skills), tr("Skills"), std::move(skillsSortCallback));


    VacancySortComparator industrySortCallback = [](const Vacancy* left, const Vacancy* right) -> bool
    {
        return industrySort(left, right);
    };
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Industry), tr("Industry"), std::move(industrySortCallback));


    VacancySortComparator expirationDateSortCallback = [](const Vacancy* left, const Vacancy* right) -> bool
    {
        return expirationDateSort(left, right);
    };
    sortMode.insertItem(static_cast<qsizetype>(SortMode::ExpirationDate), tr("Expiration Date"), std::move(expirationDateSortCallback));
}

void ResultViewOptionsPresenter::updateVisibleStatuses()
{
    emit visibleStatusesChanged(std::move(visibleStatuses()));
}

QString filterWordsToString(const QStringList& filterWords)
{
    return filterWords.join(", ");
}

QStringList filterWordsToList(const QString& filterWords)
{
    QStringList filterWordsList = filterWords.split(",");

    for (QString& word : filterWordsList)
        word = word.trimmed();

    return filterWordsList;
}
