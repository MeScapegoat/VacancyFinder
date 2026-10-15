#include "ResultViewOptionsPresenter.h"

#include <QCheckBox>
#include <QLineEdit>
#include <QStringList>

#include <CallableComboBox.hpp>
#include <ResultViewOptionsEnums.h>
#include <Vacancy.h>
#include <VacancyOperations.h>


Q_LOGGING_CATEGORY(RVOPLog, "resultViewOptions.presenter");


ResultViewOptionsPresenter::ResultViewOptionsPresenter(QObject* parent) : 
    Presenter(parent)
{
    insertFilters();
    insertSorts();

    const ResultViewOptionsModel& model = getModel();
    const ResultViewOptionsView& view = getView();


    connect(&view.filterMode, &QComboBox::currentIndexChanged, &model.currentFilterMode, &Observable<int>::setValue);
    connect(&model.currentFilterMode, &Observable<int>::valueChanged, this, &ResultViewOptionsPresenter::onCurrentFilterIndexChanged);
    onCurrentFilterIndexChanged();


    connect(&view.sortMode,         &QComboBox::currentIndexChanged, &model.currentSortMode, &Observable<int>::setValue);
    connect(&model.currentSortMode, &Observable<int>::valueChanged,  this,                   &ResultViewOptionsPresenter::onCurrentSortIndexChanged);
    onCurrentSortIndexChanged();


    connect(&view.filterLineEdit,  &QLineEdit::textChanged,                this, &ResultViewOptionsPresenter::onFilterWordsWidgetChanged);
    connect(&model.filterWordList, &Observable<QStringList>::valueChanged, this, &ResultViewOptionsPresenter::onFilterWordsChanged);
    onFilterWordsWidgetChanged();
    onFilterWordsChanged();


    connect(&view.unreviewedCheckBox,   &QCheckBox::checkStateChanged,   &model.isUnreviewedVisible, &Observable<bool>::setValue);
    connect(&model.isUnreviewedVisible, &Observable<bool>::valueChanged, this,                       &ResultViewOptionsPresenter::onUnreviewedVisibleChanged);
    onUnreviewedVisibleChanged();


    connect(&view.ignoredCheckBox,   &QCheckBox::checkStateChanged,   &model.isIgnoredVisible, &Observable<bool>::setValue);
    connect(&model.isIgnoredVisible, &Observable<bool>::valueChanged, this,                    &ResultViewOptionsPresenter::onIgnoredVisibleChanged);
    onIgnoredVisibleChanged();


    connect(&view.interestedCheckBox,   &QCheckBox::checkStateChanged,   &model.isInterestedVisible, &Observable<bool>::setValue);
    connect(&model.isInterestedVisible, &Observable<bool>::valueChanged, this,                       &ResultViewOptionsPresenter::onInterestedVisibleChanged);
    onInterestedVisibleChanged();


    QObject::connect(&model.currentFilterMode,   &AbstractObservable::valueChanged, this, &ResultViewOptionsPresenter::resultViewOptionsChanged);
    QObject::connect(&model.filterWordList,      &AbstractObservable::valueChanged, this, &ResultViewOptionsPresenter::resultViewOptionsChanged);
    QObject::connect(&model.currentSortMode,     &AbstractObservable::valueChanged, this, &ResultViewOptionsPresenter::resultViewOptionsChanged);
    QObject::connect(&model.isUnreviewedVisible, &AbstractObservable::valueChanged, this, &ResultViewOptionsPresenter::resultViewOptionsChanged);
    QObject::connect(&model.isIgnoredVisible,    &AbstractObservable::valueChanged, this, &ResultViewOptionsPresenter::resultViewOptionsChanged);
    QObject::connect(&model.isInterestedVisible, &AbstractObservable::valueChanged, this, &ResultViewOptionsPresenter::resultViewOptionsChanged);
}

void ResultViewOptionsPresenter::setFilterMode(int filterMode)
{
    getModel().currentFilterMode = filterMode;
}

int ResultViewOptionsPresenter::filterMode() const
{
    return getModel().currentFilterMode();
}

const VacancyFilter& ResultViewOptionsPresenter::vacancyFilter() const
{
    return getView().filterMode.currentFunction();
}

void ResultViewOptionsPresenter::setFilterWords(const QString& filterWords)
{
    getModel().filterWordList = filterWordsToList(filterWords);
}

QString ResultViewOptionsPresenter::filterWords() const
{
    return filterWordsToString(getModel().filterWordList());
}

void ResultViewOptionsPresenter::setSortMode(int sortMode)
{
    getModel().currentSortMode = sortMode;
}

int ResultViewOptionsPresenter::sortMode() const
{
    return getModel().currentSortMode();
}

const VacancySortComparator& ResultViewOptionsPresenter::vacancySortComparator() const
{
    return getView().sortMode.currentFunction();
}

void ResultViewOptionsPresenter::setIsUnreviewedVisible(bool isVisible)
{
    getModel().isUnreviewedVisible = isVisible;
}

bool ResultViewOptionsPresenter::isUnreviewedVisible() const
{
    return getModel().isUnreviewedVisible();
}

void ResultViewOptionsPresenter::setIsIgnoredVisible(bool isVisible)
{
    getModel().isIgnoredVisible = isVisible;
}

bool ResultViewOptionsPresenter::isIgnoredVisible() const
{
    return getModel().isIgnoredVisible();
}

void ResultViewOptionsPresenter::setIsInterestedVisible(bool isVisible)
{
    getModel().isInterestedVisible = isVisible;
}

bool ResultViewOptionsPresenter::isInterestedVisible() const
{
    return getModel().isInterestedVisible();
}

QList<VacancyStatus> ResultViewOptionsPresenter::visibleStatuses() const
{
    const ResultViewOptionsModel& model = getModel();

    QList<VacancyStatus> statuses;
    statuses.reserve(static_cast<int>(VacancyStatus::COUNT));

    if (model.isUnreviewedVisible()) statuses.push_back(VacancyStatus::Unreviewed);
    if (model.isIgnoredVisible()) statuses.push_back(VacancyStatus::Ignored);
    if (model.isInterestedVisible()) statuses.push_back(VacancyStatus::Interested);

    return statuses;
}

void ResultViewOptionsPresenter::setKeywords(const QString& keywords)
{
    getModel().setKeywords(keywords);
}

void ResultViewOptionsPresenter::onCurrentSortIndexChanged()
{
    getView().sortMode.setCurrentIndex(getModel().currentSortMode());

    emit vacancySortComparatorChanged(vacancySortComparator());
}

void ResultViewOptionsPresenter::onCurrentFilterIndexChanged()
{
    getView().filterMode.setCurrentIndex(getModel().currentFilterMode());

    emit vacancyFilterChanged(vacancyFilter());
}

void ResultViewOptionsPresenter::onFilterWordsChanged()
{
    getView().filterLineEdit.setText(filterWords());
}

void ResultViewOptionsPresenter::onFilterWordsWidgetChanged()
{
    const QString& filterWordsString = getView().filterLineEdit.text();

    getModel().filterWordList = filterWordsToList(filterWordsString);
}

void ResultViewOptionsPresenter::onUnreviewedVisibleChanged()
{
    const ResultViewOptionsModel& model = getModel();
    const ResultViewOptionsView& view = getView();

    view.unreviewedCheckBox.setChecked(model.isUnreviewedVisible());

    updateVisibleStatuses();
}

void ResultViewOptionsPresenter::onIgnoredVisibleChanged()
{
    const ResultViewOptionsModel& model = getModel();
    const ResultViewOptionsView& view = getView();

    view.ignoredCheckBox.setChecked(model.isIgnoredVisible());

    updateVisibleStatuses();
}

void ResultViewOptionsPresenter::onInterestedVisibleChanged()
{
    const ResultViewOptionsModel& model = getModel();
    const ResultViewOptionsView& view = getView();

    view.interestedCheckBox.setChecked(model.isInterestedVisible());

    updateVisibleStatuses();
}

void ResultViewOptionsPresenter::insertFilters()
{
    auto& filterMode = getView().filterMode;

    VacancyFilter offFilterCallback = [](const Vacancy* vacancy) -> bool
    {
        return offFilter();
    };
    filterMode.insertItem(static_cast<qsizetype>(FilterMode::Off), QObject::tr("Off"), std::move(offFilterCallback));


    VacancyFilter containsAnyFilterCallback = [&filterWords = getModel().filterWordList()](const Vacancy* vacancy) -> bool
    {
        return containsAnyFilter(filterWords, vacancy);
    };
    filterMode.insertItem(static_cast<qsizetype>(FilterMode::ContainsAny), QObject::tr("Contains Any"), std::move(containsAnyFilterCallback));


    VacancyFilter containsAllFilterCallback = [&filterWords = getModel().filterWordList()](const Vacancy* vacancy) -> bool
    {
        return containsAllFilter(filterWords, vacancy);
    };
    filterMode.insertItem(static_cast<qsizetype>(FilterMode::ContainsAll), QObject::tr("Contains All"), std::move(containsAllFilterCallback));


    VacancyFilter excludesAnyFilterCallback = [&filterWords = getModel().filterWordList()](const Vacancy* vacancy) -> bool
    {
        return excludesAnyFilter(filterWords, vacancy);
    };
    filterMode.insertItem(static_cast<qsizetype>(FilterMode::ExcludesAny), QObject::tr("Excludes Any"), std::move(excludesAnyFilterCallback));


    VacancyFilter excludesAllFilterCallback = [&filterWords = getModel().filterWordList()](const Vacancy* vacancy) -> bool
    {
        return excludesAllFilter(filterWords, vacancy);
    };
    filterMode.insertItem(static_cast<qsizetype>(FilterMode::ExcludesAll), QObject::tr("Excludes All"), std::move(excludesAllFilterCallback));
}

void ResultViewOptionsPresenter::insertSorts()
{
    auto& sortMode = getView().sortMode;

    VacancySortComparator offSortCallback;
    sortMode.insertItem(static_cast<qsizetype>(SortMode::Off), tr("Off"), std::move(offSortCallback));

    VacancySortComparator relevanceSortCallback = [&keywords = getModel().keywords()](const Vacancy* left, const Vacancy* right) -> bool
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