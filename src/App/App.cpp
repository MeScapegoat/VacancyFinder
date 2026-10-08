#include "App.h"

#include <QDir>
#include <QFile>
#include <QListView>
#include <QStackedLayout>
#include <QTextBrowser>
#include <QThread>
#include <QUrlQuery>

#include "AppController.h"
#include <MainWindow.h>
#include <NetworkManager.h>
#include <Parser.h>
#include <ProgressDialog.h>
#include <QueryParametersPresenter.h>
#include <RequestManager.h>
#include <RequestStrategy/LinkedInRequestStrategy.h>
#include <ResultViewOptionsPresenter.h>
#include <Vacancy.h>

#ifdef NDEBUG
#include <LinkedInParserStrategy/LinkedInParserStrategy.h>
#else
#include <LinkedInParserStrategy/LinkedInPageCollector.h>
#endif


namespace
{
    inline constexpr const char* BaseUrl = "https://www.linkedin.com/jobs-guest/jobs/api/seeMoreJobPostings/search";
}

Q_LOGGING_CATEGORY(AppLog, "app");

App::App(QObject* parent) :
    QObject(parent),
    queryParametersPresenter(*new QueryParametersPresenter(this)),
    resultViewOptionsPresenter(*new ResultViewOptionsPresenter(this)),
    controller(*new AppController(this)),
    mainWindow(*new MainWindow(queryParametersPresenter.getView().widget(), resultViewOptionsPresenter.getView().widget())),
    progressDialog(*new ProgressDialog(&mainWindow)),
    requestStrategy(new LinkedInRequestStrategy(new NetworkManager)),
    requestManager(*new RequestManager(requestStrategy)),
    networkThread(*new QThread(this)),
    parser(*new Parser),
    parserThread(*new QThread(this)),
#ifdef NDEBUG
    parserStrategy(new LinkedInParserStrategy)
#else
    parserStrategy(new LinkedInPageCollector)
#endif
{
    requestStrategy->setParent(&requestManager); // TODO: improve ownership
    parser.setStrategy(parserStrategy.get());

    // Threads
    networkThread.setObjectName("networkThread");
    requestManager.moveToThread(&networkThread);

    parserThread.setObjectName("parserThread");
    parser.moveToThread(&parserThread);

    QObject::connect(&networkThread, &QThread::finished, &requestManager, &RequestManager::deleteLater);
    QObject::connect(&parserThread, &QThread::finished, &parser, &Parser::deleteLater);


    // QueryParameters
    QObject::connect(&queryParametersPresenter, &QueryParametersPresenter::searchRequested, this, &App::sendSearchRequest);
    QObject::connect(&queryParametersPresenter, &QueryParametersPresenter::loadInterestedRequested, this, &App::loadInterested);
    QObject::connect(&queryParametersPresenter, &QueryParametersPresenter::resetRequested, this, &App::reset);


    // ResultViewOptions
    ResultViewOptionsModel& RVOModel = resultViewOptionsPresenter.getModel();

    QObject::connect(&RVOModel.currentSortMode,   &Observable<int>::valueChanged,     this, &App::updateDescriptionList);

    QObject::connect(&RVOModel.currentFilterMode, &Observable<int>::valueChanged,     this, &App::updateDescriptionList);
    QObject::connect(&RVOModel.filterText,        &Observable<QString>::valueChanged, this, &App::updateDescriptionList);

    QObject::connect(&RVOModel.isUnreviewedVisible, &Observable<bool>::valueChanged, this, &App::updateDescriptionList);
    QObject::connect(&RVOModel.isIgnoredVisible,    &Observable<bool>::valueChanged, this, &App::updateDescriptionList);
    QObject::connect(&RVOModel.isInterestedVisible, &Observable<bool>::valueChanged, this, &App::updateDescriptionList);

    // RequestManager
    QObject::connect(&requestManager, &RequestManager::requestsStarted, &controller, &AppController::workStarted);
    QObject::connect(&requestManager, &RequestManager::requestsFinished, &controller, &AppController::workFinished);

    //QObject::connect(&requestManager, &RequestManager::meaningfulRepliesReceived, this, &App::onMeaningfulRepliesReceived); // -1 network work +1 parsing work, so the total work remains unchanged
    QObject::connect(&requestManager, &RequestManager::meaninglessRepliesReceived, &controller, &AppController::workFinished);
    
    QObject::connect(&requestManager, &RequestManager::receivedReply, &parser, &Parser::processPage);
    QObject::connect(&requestManager, &RequestManager::vacancyCountDetermined, &progressDialog, &ProgressDialog::updateVacancyCount);

    QObject::connect(&requestManager, &RequestManager::urlsEnqueued, &controller, &AppController::workStarted);
    QObject::connect(&requestManager, &RequestManager::urlsDequeued, &controller, &AppController::workFinished);


    // Parser
    QObject::connect(&parser, &Parser::requestDetails, &requestManager, &RequestManager::addUrl);

    QObject::connect(&parser, &Parser::briefParsed, this, &App::onBriefReady);
    QObject::connect(&parser, &Parser::briefParsed, &progressDialog, &ProgressDialog::updateBrief);

    QObject::connect(&parser, &Parser::detailsParsed, this, &App::onDetailsReady);
    QObject::connect(&parser, &Parser::detailsParsed, &progressDialog, &ProgressDialog::updateDetails);

    //QObject::connect(&parser, &Parser::pageParsingStarted,  &controller, &AppController::workStarted); // Work is actually added in App::onMeaningfulRepliesReceived
    QObject::connect(&parser, &Parser::pageParsingFinished, &controller, &AppController::workFinished);


    // MainWindow
    QObject::connect(&mainWindow.vacanciesList,        &QListView::clicked,    this, &App::openVacancyDescription);

    QObject::connect(&mainWindow.ignoreButton,         &QPushButton::released, this, &App::onIgnoreButtonPressed);
    QObject::connect(&mainWindow.interestButton,       &QPushButton::released, this, &App::onInterestButtonPressed);

    QObject::connect(&mainWindow.removeIgnoreButton,   &QPushButton::released, this, &App::onRemoveIgnoreButtonPressed);
    QObject::connect(&mainWindow.removeInterestButton, &QPushButton::released, this, &App::onRemoveInterestButtonPressed);


    // ProgressDialog
    QObject::connect(&progressDialog.stopButton, &QPushButton::released, &requestManager, &RequestManager::interrupt);
    QObject::connect(&progressDialog.stopButton, &QPushButton::released, &parser, &Parser::interrupt);

    // AppController
    QObject::connect(&controller, &AppController::allActiveWorkFinished, this, &App::updateDescriptionList);
    QObject::connect(&controller, &AppController::allActiveWorkFinished, &progressDialog, &ProgressDialog::accept);

    // This
    QObject::connect(this, &App::initSearch, &requestManager, &RequestManager::addUrls);    
    QObject::connect(this, &App::updateVacanciesList, &mainWindow, &MainWindow::updateVacanciesList);
    QObject::connect(this, &App::openProgressDialog, &progressDialog, &ProgressDialog::start);

    
    loadState();

    closeVacancyDescription();

    networkThread.start();
    parserThread.start();
}

App::~App()
{
    saveState();

    networkThread.quit();
    networkThread.wait();

    parserThread.quit();
    parserThread.wait();
}

void App::reset()
{
    vacancies.clear();
    vacanciesToShow.clear();

    controller.reset();
    requestManager.reset();
    parser.reset();
}

void App::loadInterested()
{
    updateMaxVacancies(interestedVacancies.size());

    searchStartTime = QTime::currentTime();

    emit initSearch(interestedVacancies.values().toVector());
    emit openProgressDialog(maxVacancies);
}

void App::sendSearchRequest()
{    
    updateMaxVacancies(queryParametersPresenter.getModel().maxResults());

    controller.reset();

    keywords = queryParametersPresenter.getModel().keywords();
    location = queryParametersPresenter.getModel().location();

    qCInfo(AppLog) << "keywords:" << keywords << "location:" << location;

    QVector<QUrl> urls;

    for (auto n = 0; n < maxVacancies; n += 10)
    {
        QUrl url(BaseUrl);
        QUrlQuery query;
        query.addQueryItem("keywords", QUrl::toPercentEncoding(keywords));
        query.addQueryItem("location", QUrl::toPercentEncoding(location));
        query.addQueryItem("start", QString::number(n));
        url.setQuery(query);

        urls.append(url);
    }

    searchStartTime = QTime::currentTime();

    emit initSearch(urls);
    emit openProgressDialog(maxVacancies);
}

void App::openVacancyDescription(const QModelIndex& index)
{
    const int id = index.row();
    const Vacancy& vacancy = *vacanciesToShow[id];

    const VacancyStatus status = getVacancyStatus(vacancy);
    mainWindow.statusButtonsLayout.setCurrentIndex(static_cast<int>(status));

    const QString& title = vacancy.title;
    const QString& description = vacancy.description;
    const std::optional<QString>& level = vacancy.level;
    const std::optional<Months>& experience = vacancy.experience;
    const std::optional<QString>& industry = vacancy.industry;
    const std::optional<QString>& education = vacancy.education;
    const std::optional<QDate>& expirationDate = vacancy.expirationDate;
    const QString& skills = vacancy.skills;
    const QString& employmentType = vacancy.employmentType;
    const QString& companyName = vacancy.companyName;
    const QString& companyLink = vacancy.companyLink.toString();

    qCInfo(AppLog) << "Loading descrption:" << id << "." << title;

    const QString linkPattern = "<a href='%1'>%2</a>";

    mainWindow.titleLabel.setText(linkPattern.arg(vacancy.link.toString()).arg(title));
    mainWindow.companyNameLabel.setText(linkPattern.arg(companyLink).arg(companyName));

    mainWindow.employmentTypeLabel.setText("Employment type: " + employmentType);


    mainWindow.levelLabel.setVisible(level.has_value());
    if (level) mainWindow.levelLabel.setText("Level: " + *level);

    mainWindow.experienceLabel.setVisible(experience.has_value());
    if (experience) mainWindow.experienceLabel.setText("Experience: " + QString::number(*experience / 12.0) + " years");

    mainWindow.educationLabel.setVisible(education.has_value());
    if (education) mainWindow.educationLabel.setText("Education: " + *education);

    mainWindow.skillsLabel.setVisible(!skills.isEmpty());
    if (!skills.isEmpty()) mainWindow.skillsLabel.setText("Skills: " + skills);

    mainWindow.industryLabel.setVisible(industry.has_value());
    if (industry) mainWindow.industryLabel.setText("Industry: " + *industry);

    mainWindow.expirationDateLabel.setVisible(expirationDate.has_value());
    if (expirationDate) mainWindow.expirationDateLabel.setText("Expiration date: " + expirationDate->toString("dd-MM-yyyy"));

    mainWindow.vacancyDesc.setHtml(description);
}

void App::closeVacancyDescription()
{
    mainWindow.titleLabel.setText("Title");
    mainWindow.companyNameLabel.setText("Company");

    mainWindow.employmentTypeLabel.setText("Employment type");

    mainWindow.levelLabel.setVisible(true);
    mainWindow.levelLabel.setText("Level");

    mainWindow.experienceLabel.setVisible(true);
    mainWindow.experienceLabel.setText("Experience");

    mainWindow.educationLabel.setVisible(true);
    mainWindow.educationLabel.setText("Education");

    mainWindow.skillsLabel.setVisible(true);
    mainWindow.skillsLabel.setText("Skills");

    mainWindow.industryLabel.setVisible(true);
    mainWindow.industryLabel.setText("Industry");

    mainWindow.expirationDateLabel.setVisible(true);
    mainWindow.expirationDateLabel.setText("Expiration date");


    mainWindow.vacancyDesc.setHtml(R"(
        <!DOCTYPE html>
        <html lang="en">
        <body>
            <header>
                <h1>Choose vacancy from left list to see its description here</h1>
            </header>
        </body>
        </html>)");

    mainWindow.statusButtonsLayout.setCurrentIndex(static_cast<int>(VacancyStatus::Unreviewed));
}

void App::updateDescriptionList()
{

    mainWindow.clearList();


    syncVacanciesToShow();

    applyStatusFilter();

    applyFiltering();

    applySorting();

    updateModelData();
}

void App::updateMaxVacancies(const int maxVacancies_)
{
    maxVacancies = maxVacancies_;
}

void App::onBriefReady(const Vacancy& vacancy)
{
    auto it = vacancies.find(vacancy.link);
    if (it == vacancies.end())
    {
        vacancies.emplace(vacancy.link, vacancy);
    }
    else
    {
        Vacancy& foundVacancy = it.value();
        foundVacancy.fillEmptyFrom(vacancy);

        qCDebug(AppLog) << foundVacancy;
    }
}

void App::onDetailsReady(const Vacancy& vacancy)
{
    auto it = vacancies.find(vacancy.link);
    if (it == vacancies.end())
    {
        vacancies.emplace(vacancy.link, vacancy);
    }
    else
    {
        Vacancy& foundVacancy = it.value();
        foundVacancy.fillEmptyFrom(vacancy);

        qCDebug(AppLog) << foundVacancy;
    }
}

void App::onIgnoreButtonPressed()
{
    const std::optional<constVacancyRef> vacancy = getCurrentVacancy();
    if (!vacancy)
        return;

    ignoredVacancies.insert(vacancy->get().link);

    updateDescriptionList();
    mainWindow.statusButtonsLayout.setCurrentIndex(static_cast<int>(VacancyStatus::Ignored));
    closeVacancyDescription();
}

void App::onInterestButtonPressed()
{
    const std::optional<constVacancyRef> vacancy = getCurrentVacancy();
    if (!vacancy)
        return;

    interestedVacancies.insert(vacancy->get().link);

    updateDescriptionList();
    mainWindow.statusButtonsLayout.setCurrentIndex(static_cast<int>(VacancyStatus::Interested));
    closeVacancyDescription();
}

void App::onRemoveIgnoreButtonPressed()
{
    const std::optional<constVacancyRef> vacancy = getCurrentVacancy();
    if (!vacancy)
        return;

    ignoredVacancies.remove(vacancy->get().link);

    updateDescriptionList();
    mainWindow.statusButtonsLayout.setCurrentIndex(static_cast<int>(VacancyStatus::Unreviewed));
    closeVacancyDescription();
}

void App::onRemoveInterestButtonPressed()
{
    const std::optional<constVacancyRef> vacancy = getCurrentVacancy();
    if (!vacancy)
        return;

    interestedVacancies.remove(vacancy->get().link);

    updateDescriptionList();
    mainWindow.statusButtonsLayout.setCurrentIndex(static_cast<int>(VacancyStatus::Unreviewed));
    closeVacancyDescription();
}

void App::onMeaningfulRepliesReceived(int amount)
{
    controller.workStarted(amount);
    controller.workFinished(amount);
}

QString App::loadTitleFromFile(const QString& path)
{
    QFile file(path);
    QString ret;
    if (file.open(QIODeviceBase::ReadOnly))
    {
        ret = file.readAll();
        file.close();
    }
    else
        qCWarning(AppLog) << "couldn't open " << path;
    return ret;
}

QString App::loadDescriptionFromFile(const QString& path)
{
    QFile file(path);
    QString ret;
    if (file.open(QIODeviceBase::ReadOnly))
    {
        ret = file.readAll();
        file.close();
    }
    else
        qCWarning(AppLog) << "couldn't open " << path;
    return ret;
}

QHash<QUrl, Vacancy>& App::getVacancies()
{
    return vacancies;
}

const QHash<QUrl, Vacancy>& App::getVacancies() const
{
    return vacancies;
}

const QList<const Vacancy*>& App::getVacanciestoShow() const
{
    return vacanciesToShow;
}

void App::syncVacanciesToShow()
{
    vacanciesToShow.clear();
    for (const Vacancy& vacancy : vacancies)
    {
        vacanciesToShow.push_back(&vacancy);
    }
}

void App::applyStatusFilter()
{
    const bool isUnreviewedVisible = resultViewOptionsPresenter.getModel().isUnreviewedVisible();
    const bool isIgnoredVisible = resultViewOptionsPresenter.getModel().isIgnoredVisible();
    const bool isInterestedVisible = resultViewOptionsPresenter.getModel().isInterestedVisible();

    // TODO: std::erase_if
    for (auto it = vacanciesToShow.begin(); it != vacanciesToShow.end(); /* EMPTY */)
    {
        const Vacancy& vacancy = **it;
        const VacancyStatus status = getVacancyStatus(vacancy);
        bool isRemoving = false;

        switch (status)
        {
            case VacancyStatus::Ignored:
            {
                isRemoving = !isIgnoredVisible;
                break;
            }
            case VacancyStatus::Interested:
            {
                isRemoving = !isInterestedVisible;
                break;
            }
            case VacancyStatus::Unreviewed:
            {
                isRemoving = !isUnreviewedVisible;
                break;
            }
        }

        if (isRemoving)
            it = vacanciesToShow.erase(it);
        else
            ++it;
    }
}

void App::applyFiltering()
{
    const FilterMode filterMode = static_cast<FilterMode>(resultViewOptionsPresenter.getModel().currentFilterMode());

    const QString& filterText = resultViewOptionsPresenter.getModel().filterText();

    QStringList filterWords = filterText.split(',');
    if (filterWords.isEmpty()) return;

    for (QString& word : filterWords)
        word = word.trimmed();

    switch (filterMode)
    {
        case FilterMode::Off:
        {
            return;
        }
        case FilterMode::Include:
        {
            // TODO: std::erase_if
            qCDebug(AppLog) << "Include filter mode, filter words:" << filterWords;
            for (auto it = vacanciesToShow.begin(); it != vacanciesToShow.end(); /* EMPTY */)
            {
                const Vacancy& vacancy = **it;
                bool isFilterOk = true;
                for (const QString& word : filterWords)
                {
                    if (vacancy.count(word) == 0)
                    {
                        isFilterOk = false;
                        break;
                    }
                }

                if (!isFilterOk)
                    it = vacanciesToShow.erase(it);
                else
                    ++it;
            }
            break;
        }
        case FilterMode::Exclude:
        {
            // TODO: std::erase_if
            qCDebug(AppLog) << "Exclude filter mode, filter words:" << filterWords;
            for (auto it = vacanciesToShow.begin(); it != vacanciesToShow.end(); /* EMPTY */)
            {
                const Vacancy& vacancy = **it;
                bool isFilterOk = true;
                for (const QString& word : filterWords)
                {
                    if (vacancy.count(word) != 0)
                    {
                        isFilterOk = false;
                        break;
                    }
                }

                if (!isFilterOk)
                    it = vacanciesToShow.erase(it);
                else
                    ++it;
            }
            break;
        }
    }
}

void App::applySorting()
{
    const SortMode& sortMode = static_cast<SortMode>(resultViewOptionsPresenter.getModel().currentSortMode());

    switch (sortMode)
    {
        case SortMode::Off:
        {
            return;
        }
        case SortMode::Relevance:
        {
            sortByRelevance();
            break;
        }
        case SortMode::Title:
        {
            sortByTitle();
            break;
        }
        case SortMode::Company:
        {
            sortByCompany();
            break;
        }
        case SortMode::Time:
        {
            sortByTime();
            break;
        }
        case SortMode::EmploymentType:
        {
            sortByEmploymentType();
            break;
        }
        case SortMode::Level:
        {
            sortByLevel();
            break;
        }
        case SortMode::Experience:
        {
            sortByExperience();
            break;
        }
        case SortMode::Education:
        {
            sortByEducation();
            break;
        }
        case SortMode::Skills:
        {
            sortBySkills();
            break;
        }
        case SortMode::Industry:
        {
            sortByIndustry();
            break;
        }
        case SortMode::ExpirationDate:
        {
            sortByExpirationDate();
            break;
        }
    }
}

void App::updateModelData()
{
    QStringList data;

    for (const Vacancy* vacancy : vacanciesToShow)
    {                            
        const QString& id = QString::number(data.size() + 1);
        const QString& title = vacancy->title;
        const QString& companyName = vacancy->companyName;
        const qsizetype& companyNameWidth = companyName.size() + 8;
        const QString& age = QString::number(vacancy->date.daysTo(QDate::currentDate()));
        const QString& location = vacancy->location;

        data.append(QString("%1. %2\n%3\n%4\n%5 days ago")
                    .arg(id.leftJustified(3))
                    .arg(title)
                    .arg(companyName)
                    .arg(location)
                    .arg(age));
    }

    emit updateVacanciesList(data);
}

const unsigned& App::getMaxVacancies() const
{
    return maxVacancies;
}

void App::sortByRelevance()
{
    qCDebug(AppLog) << "Sorting by relevance";

    auto compareByRelevance = [&](const Vacancy* left, const Vacancy* right)
    {
        return left->count(keywords) > right->count(keywords);
    };

    std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareByRelevance);
}

void App::sortByTitle()
{
    qCDebug(AppLog) << "Sorting by title";

    auto compareByTitle = [](const Vacancy* left, const Vacancy* right)
    {
        return left->title < right->title;
    };

    std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareByTitle);
}

void App::sortByCompany()
{
    qCDebug(AppLog) << "Sorting by company";

    auto compareByCompany = [](const Vacancy* left, const Vacancy* right)
    {
        return left->companyName < right->companyName;
    };

    std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareByCompany);
}

void App::sortByTime()
{
    qCDebug(AppLog) << "Sorting by time";

    auto compareByTime = [](const Vacancy* left, const Vacancy* right)
    {
        return left->date > right->date;
    };

    std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareByTime);
}

void App::sortByEmploymentType()
{
    qCDebug(AppLog) << "sorting by Employment Type";
    auto compareByEmploymentType = [](const Vacancy* left, const Vacancy* right)
    {
        return left->employmentType > right->employmentType;
    };

    std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareByEmploymentType);
}

void App::sortByLevel()
{
    qCDebug(AppLog) << "Sorting by level";

    auto compareByLevel = [&](const Vacancy* left, const Vacancy* right)
    {
        if (!left->level.has_value()) return false;
        if (!right->level.has_value()) return true;

        return left->level < right->level;
    };

    std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareByLevel);
}

void App::sortByExperience()
{
    qCDebug(AppLog) << "Sorting by experience";

    auto compareByExperience = [&](const Vacancy* left, const Vacancy* right)
    {
        if (!left->experience.has_value()) return false;
        if (!right->experience.has_value()) return true;

        return left->experience < right->experience;
    };

    std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareByExperience);
}

void App::sortByEducation()
{
    qCDebug(AppLog) << "Sorting by education";

    auto compareByEducation = [&](const Vacancy* left, const Vacancy* right)
    {
        if (!left->education.has_value()) return false;
        if (!right->education.has_value()) return true;

        return left->education < right->education;
    };

    std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareByEducation);
}

void App::sortBySkills()
{
    qCDebug(AppLog) << "Sorting by skills";

    auto compareBySkills = [&](const Vacancy* left, const Vacancy* right)
    {
        if (left->skills.isEmpty()) return false;
        if (right->skills.isEmpty()) return true;

        return left->skills < right->skills;
    };

    std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareBySkills);
}

void App::sortByIndustry()
{
    qCDebug(AppLog) << "sorting by industry";

    auto compareByIndustry = [&](const Vacancy* left, const Vacancy* right)
    {
        if (!left->industry.has_value()) return false;
        if (!right->industry.has_value()) return true;

        return left->industry < right->industry;
    };

    std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareByIndustry);
}

void App::sortByExpirationDate()
{
    qCDebug(AppLog) << "sorting by Relevance";

    auto compareByExpirationDate = [&](const Vacancy* left, const Vacancy* right)
    {
        if (!left->expirationDate.has_value()) return false;
        if (!right->expirationDate.has_value()) return true;

        return left->expirationDate < right->expirationDate;
    };

    std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareByExpirationDate);
}

std::optional<App::constVacancyRef> App::getCurrentVacancy() const
{
    const QModelIndex& index = mainWindow.vacanciesList.currentIndex();

    if (!index.isValid())
        return std::nullopt;

    const int id = index.row();

    return *vacanciesToShow[id];
}

VacancyStatus App::getVacancyStatus(const Vacancy& vacancy)
{
    const QUrl& url = vacancy.link;

    if (interestedVacancies.contains(url))
        return VacancyStatus::Interested;

    if (ignoredVacancies.contains(url))
        return VacancyStatus::Ignored;

    return VacancyStatus::Unreviewed;
}

void App::saveState()
{
    state.setIgnored(ignoredVacancies);
    state.setInterested(interestedVacancies);

    state.setKeywords(queryParametersPresenter.getModel().keywords());
    state.setLocation(queryParametersPresenter.getModel().location());

    state.setLimit(queryParametersPresenter.getModel().maxResults());

    state.setFilterMode(resultViewOptionsPresenter.getModel().currentFilterMode());
    state.setFilters(resultViewOptionsPresenter.getModel().filterText());

    state.setSortMode(resultViewOptionsPresenter.getModel().currentSortMode());

    state.setIsUnreviewedVisible(resultViewOptionsPresenter.getModel().isUnreviewedVisible());
    state.setIsIgnoredVisible(resultViewOptionsPresenter.getModel().isIgnoredVisible());
    state.setIsInterestedVisible(resultViewOptionsPresenter.getModel().isInterestedVisible());


    state.saveState();
}

void App::loadState()
{
    state.loadState();

    ignoredVacancies = state.getIgnored();
    interestedVacancies = state.getInterested();

    queryParametersPresenter.getModel().keywords = state.getKeywords();
    queryParametersPresenter.getModel().location = state.getLocation();

    queryParametersPresenter.getModel().maxResults = state.getLimit();

    resultViewOptionsPresenter.getModel().currentFilterMode = state.getFilterMode();
    resultViewOptionsPresenter.getModel().filterText = state.getFilters();

    resultViewOptionsPresenter.getModel().currentSortMode = state.getSortMode();


    resultViewOptionsPresenter.getModel().isUnreviewedVisible = state.getIsUnreviewedVisible();
    resultViewOptionsPresenter.getModel().isIgnoredVisible = state.getIsIgnoredVisible();
    resultViewOptionsPresenter.getModel().isInterestedVisible = state.getIsInterestedVisible();
}