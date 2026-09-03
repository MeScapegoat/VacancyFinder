// std

// Qt
#include <QButtonGroup>
#include <QCheckBox>
#include <QComboBox>
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QLineEdit>
#include <QListView>
#include <QSpinBox>
#include <QTextBrowser>
#include <QUrlQuery>
#include <QStackedLayout>

// custom
#include "../network/RequestManager.h"
#include "App.h"

namespace
{
    inline constexpr const char* BaseUrl = "https://www.linkedin.com/jobs-guest/jobs/api/seeMoreJobPostings/search";
}

namespace Linked
{
    Q_LOGGING_CATEGORY(LinkedApp, "app");

    App::App(QObject* parent) :
        QObject(parent),
        context(new SharedContext),
        requestManager(*new RequestManager),
        networkThread(new QThread(this)),
        parser(new HTMLparser),
        parserThread(new QThread(this)),
        gui(new GUI(context))
    {
        networkThread->setObjectName("networkThread");
        requestManager.moveToThread(networkThread);

        parserThread->setObjectName("parserThread");
        parser->moveToThread(parserThread);

        context->app = this;
        context->mainWindow = gui->mainWindow();

        QObject::connect(networkThread, &QThread::finished, &requestManager, &RequestManager::deleteLater);
        QObject::connect(parserThread, &QThread::finished, parser, &HTMLparser::deleteLater);

        QObject::connect(&gui->mainWindow()->limitVacanciesInput, &QSpinBox::valueChanged, this, &App::updateMaxVacancies);

        QObject::connect(&requestManager, &RequestManager::requestsFinished, this, &App::onRequestsFinished);
        QObject::connect(&requestManager, &RequestManager::requestsFinished, parser, &HTMLparser::onRequestsFinished);

        QObject::connect(&requestManager, &RequestManager::receivedReply, parser, &HTMLparser::processHTML);
        QObject::connect(this, &App::initSearch, &requestManager, &RequestManager::addUrls);

        QObject::connect(parser, &HTMLparser::sendRequest, &requestManager, &RequestManager::addUrl);

        QObject::connect(parser, &HTMLparser::briefsFinished, this, &App::onBriefsFinished);
        QObject::connect(parser, &HTMLparser::allDetailsFinished, this, &App::onAllDetailsFinished);

        QObject::connect(parser, &HTMLparser::briefReady, this, &App::onBriefReady);
        QObject::connect(parser, &HTMLparser::detailsReady, this, &App::onDetailsReady);

        QObject::connect(parser, &HTMLparser::briefReady, gui->progressDialog(), &ProgressDialog::updateBrief);
        QObject::connect(parser, &HTMLparser::detailsReady, gui->progressDialog(), &ProgressDialog::updateDetails);

        QObject::connect(this, &App::updateVacanciesList, gui->mainWindow(), &MainWindow::updateVacanciesList);
        QObject::connect(&gui->mainWindow()->vacanciesList, &QListView::clicked, this, &App::openVacancyDescription);
        QObject::connect(&gui->mainWindow()->searchButton, &QPushButton::released, this, &App::sendSearchRequest);

        QObject::connect(&gui->mainWindow()->ignoreButton, &QPushButton::released, this, &App::onIgnoreButtonPressed);
        QObject::connect(&gui->mainWindow()->interestButton, &QPushButton::released, this, &App::onInterestButtonPressed);
        QObject::connect(&gui->mainWindow()->removeIgnoreButton,   &QPushButton::released, this, &App::onRemoveIgnoreButtonPressed);
        QObject::connect(&gui->mainWindow()->removeInterestButton, &QPushButton::released, this, &App::onRemoveInterestButtonPressed);

        QObject::connect(this, &App::openProgressDialog, gui->progressDialog(), &ProgressDialog::start);
        QObject::connect(this, &App::closeProgressDialog, gui->progressDialog(), &ProgressDialog::accept);

        QObject::connect(&gui->mainWindow()->sortMode, &QComboBox::currentIndexChanged, this, &App::updateDescriptionList);
        QObject::connect(&gui->mainWindow()->filterMode, &QComboBox::currentIndexChanged, this, &App::updateDescriptionList);
        QObject::connect(&gui->mainWindow()->filterLineEdit, &QLineEdit::editingFinished, [&]()
        {
            updateDescriptionList(0);
        });
        QObject::connect(&gui->mainWindow()->unreviewedCheckBox, &QCheckBox::checkStateChanged, this, &App::updateDescriptionList);
        QObject::connect(&gui->mainWindow()->ignoredCheckBox, &QCheckBox::checkStateChanged, this, &App::updateDescriptionList);
        QObject::connect(&gui->mainWindow()->interestedCheckBox, &QCheckBox::checkStateChanged, this, &App::updateDescriptionList);

        QObject::connect(&requestManager, &RequestManager::replyError, gui->progressDialog(), &ProgressDialog::updateError);

        QObject::connect(&gui->progressDialog()->stopButton, &QPushButton::released, &requestManager, &RequestManager::onInterrupt);
        QObject::connect(&gui->progressDialog()->stopButton, &QPushButton::released, parser, &HTMLparser::onInterrupt);


        loadState();

        closeVacancyDescription();

        networkThread->start();
        parserThread->start();
    }

    App::~App()
    {
        saveState();

        networkThread->quit();
        networkThread->wait();

        parserThread->quit();
        parserThread->wait();

        delete context;
    }

    void App::sendSearchRequest()
    {
        vacancies.clear();
        vacanciesToShow.clear();
        requestManager.reset();
        parser->reset();

        updateMaxVacancies(gui->mainWindow()->limitVacanciesInput.value());

        isRequestsFinished = false;
        isBriefsFinished = false;
        isAllDetailsFinished = false;

        keywords = gui->mainWindow()->keywordsLineEdit.text();
        location = gui->mainWindow()->locationLineEdit.text();

        qCInfo(LinkedApp) << "keywords:" << keywords << "location:" << location;

        QVector<QUrl> urls;

        if (maxVacancies > 10)
        {
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
        }

        QUrl url(BaseUrl);
        QUrlQuery query;
        query.addQueryItem("keywords", QUrl::toPercentEncoding(keywords));
        query.addQueryItem("location", QUrl::toPercentEncoding(location));
        query.addQueryItem("start", QString::number(maxVacancies));
        url.setQuery(query);
        urls.append(url);

        //QUrl url1(baseUrl);
        //QUrlQuery query1;
        //query1.addQueryItem("keywords", QUrl::toPercentEncoding(keywords));
        //query1.addQueryItem("location", QUrl::toPercentEncoding(location));
        //query1.addQueryItem("start", QString::number(20));
        //url1.setQuery(query1);
        //
        //QUrl url2(baseUrl);
        //QUrlQuery query2;
        //query2.addQueryItem("keywords", QUrl::toPercentEncoding(keywords));
        //query2.addQueryItem("location", QUrl::toPercentEncoding(location));
        //query2.addQueryItem("start", QString::number(20));
        //url2.setQuery(query2);
        //
        //QUrl url3(baseUrl);
        //QUrlQuery query3;
        //query3.addQueryItem("keywords", QUrl::toPercentEncoding(keywords));
        //query3.addQueryItem("location", QUrl::toPercentEncoding(location));
        //query3.addQueryItem("start", QString::number(20));
        //url3.setQuery(query3);
        //
        //QUrl url4(baseUrl);
        //QUrlQuery query4;
        //query4.addQueryItem("keywords", QUrl::toPercentEncoding(keywords));
        //query4.addQueryItem("location", QUrl::toPercentEncoding(location));
        //query4.addQueryItem("start", QString::number(20));
        //url4.setQuery(query4);
        //
        //urls.append(url1);
        //urls.append(url2);
        //urls.append(url3);
        //urls.append(url4);

        searchStartTime = QTime::currentTime();

        qCInfo(LinkedApp) << QThread::currentThread();

        emit initSearch(urls);
        emit openProgressDialog(maxVacancies);
    }

    void App::openVacancyDescription(const QModelIndex& index)
    {
        const int id = index.row();
        const Vacancy& vacancy = *vacanciesToShow[id];

        const VacancyStatus status = getVacancyStatus(vacancy);
        gui->mainWindow()->statusButtonsLayout.setCurrentIndex(static_cast<int>(status));

        const QString& title = vacancy.brief.title;
        const QString& description = vacancy.details.description;
        const std::optional<QString>& level = vacancy.details.level;
        const std::optional<Months>& experience = vacancy.details.experience;
        const std::optional<QString>& industry = vacancy.details.industry;
        const std::optional<QString>& education = vacancy.details.education;
        const std::optional<QDate>& expirationDate = vacancy.details.expirationDate;
        const QString& skills = vacancy.details.skills;
        const QString& employmentType = vacancy.details.employmentType;
        const QString& companyName = vacancy.brief.companyName;
        const QString& companyLink = vacancy.brief.companyLink.toString();

        qCInfo(LinkedApp) << "Loading descrption:" << id << "." << title;

        //const QString HTMLformated =  "<h1>Description:</h1>" + description +
        //                        "<h1>Level:</h1>" + level +
        //                        "<h1>employmentType:</h1>" + employmentType;

        //QString HTMLdebug = "<h1>description</h1>" + vacancy.description +
        //    "<h1>title</h1>" + vacancy.title +
        //    "<h1>link</h1>" + vacancy.link.toString() +
        //    "<h1>companyName</h1>" + vacancy.companyName +
        //    "<h1>companyLink</h1>" + vacancy.companyLink.toString() +
        //    "<h1>date</h1>" + vacancy.date.toString() +
        //    "<h1>location</h1>" + vacancy.location +
        //    "<h1>level</h1>" + vacancy.level +
        //    "<h1>employmentType</h1>" + vacancy.employmentType;

        const MainWindow& mainWindow = *gui->mainWindow();

        const QString linkPattern = "<a href='%1'>%2</a>";

        mainWindow.titleLabel.setText(linkPattern.arg(vacancy.brief.link.toString()).arg(title));
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
        MainWindow& mainWindow = *gui->mainWindow();


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

    void App::updateDescriptionList(int temp)
    {
        gui->mainWindow()->clearList();


        syncVacanciesToShow();

        applyStatusFilter();

        applyFiltering();

        applySorting();

        updateModelData();
    }

    void App::updateMaxVacancies(const unsigned newMaxVacancies)
    {
        maxVacancies = newMaxVacancies;
        parser->setMaxVacancies(maxVacancies);
    }

    void App::onBriefReady(const VacancyBrief& brief)
    {
        auto it = vacancies.find(brief.link);
        if (it == vacancies.end())
        {
            vacancies.emplace(brief.link, brief);
        }
        else
        {
            Vacancy& foundVacancy = it.value();
            foundVacancy.brief = brief;

            qCDebug(LinkedApp) << foundVacancy;
        }
    }

    void App::onDetailsReady(const VacancyDetails& details)
    {
        auto it = vacancies.find(details.link);
        if (it == vacancies.end())
        {
            vacancies.emplace(details.link, details);
        }
        else
        {
            Vacancy& foundVacancy = it.value();
            foundVacancy.details = details;

            qCDebug(LinkedApp) << foundVacancy;
        }
    }

    void App::onBriefsFinished()
    {
        isBriefsFinished = true;
        updateIsAllFinished();
    }

    void App::onAllDetailsFinished()
    {
        isAllDetailsFinished = true;
        updateIsAllFinished();
    }

    void App::onRequestsFinished()
    {
        isRequestsFinished = true;
        updateIsAllFinished();
    }

    void App::onIgnoreButtonPressed()
    {
        const std::optional<constVacancyRef> vacancy = getCurrentVacancy();
        if (!vacancy)
            return;

        ignoredVacancies.insert(vacancy->get().brief.link);

        updateDescriptionList();
        gui->mainWindow()->statusButtonsLayout.setCurrentIndex(static_cast<int>(VacancyStatus::Ignored));
        closeVacancyDescription();
    }

    void App::onInterestButtonPressed()
    {
        const std::optional<constVacancyRef> vacancy = getCurrentVacancy();
        if (!vacancy)
            return;

        interestedVacancies.insert(vacancy->get().brief.link);

        updateDescriptionList();
        gui->mainWindow()->statusButtonsLayout.setCurrentIndex(static_cast<int>(VacancyStatus::Interested));
        closeVacancyDescription();
    }

    void App::onRemoveIgnoreButtonPressed()
    {
        const std::optional<constVacancyRef> vacancy = getCurrentVacancy();
        if (!vacancy)
            return;

        ignoredVacancies.remove(vacancy->get().brief.link);

        updateDescriptionList();
        gui->mainWindow()->statusButtonsLayout.setCurrentIndex(static_cast<int>(VacancyStatus::Unreviewed));
        closeVacancyDescription();
    }

    void App::onRemoveInterestButtonPressed()
    {
        const std::optional<constVacancyRef> vacancy = getCurrentVacancy();
        if (!vacancy)
            return;

        interestedVacancies.remove(vacancy->get().brief.link);

        updateDescriptionList();
        gui->mainWindow()->statusButtonsLayout.setCurrentIndex(static_cast<int>(VacancyStatus::Unreviewed));
        closeVacancyDescription();
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
            qCWarning(LinkedApp) << "couldn't open " << path;
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
            qCWarning(LinkedApp) << "couldn't open " << path;
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

    void App::updateIsAllFinished()
    {
        isAllFinished = isRequestsFinished && isBriefsFinished && isAllDetailsFinished;

        if (!isAllFinished)
        {
            return;
        }

        qCInfo(LinkedApp) << "Time spent for" << vacancies.size() << "vacancies is" << searchStartTime.secsTo(QTime::currentTime()) << "seconds";

        updateDescriptionList();

        emit closeProgressDialog();
    }

    void App::applyStatusFilter()
    {
        const MainWindow& mainWindow = *gui->mainWindow();

        const bool isUnreviewedVisible = mainWindow.unreviewedCheckBox.isChecked();
        const bool isIgnoredVisible = mainWindow.ignoredCheckBox.isChecked();
        const bool isInterestedVisible = mainWindow.interestedCheckBox.isChecked();

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
        const MainWindow& mainWindow = *gui->mainWindow();

        const FilterMode filterMode = static_cast<FilterMode>(mainWindow.filterMode.currentIndex());

        const QString& filterText = gui->mainWindow()->filterLineEdit.text();

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
                qCDebug(LinkedApp) << "Include filter mode, filter words:" << filterWords;
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
                qCDebug(LinkedApp) << "Exclude filter mode, filter words:" << filterWords;
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
        const SortMode& sortMode = static_cast<SortMode>(gui->mainWindow()->sortMode.currentIndex());

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
            const QString& title = vacancy->brief.title;
            const QString& companyName = vacancy->brief.companyName;
            const qsizetype& companyNameWidth = companyName.size() + 8;
            const QString& age = QString::number(vacancy->brief.date.daysTo(QDate::currentDate()));
            const QString& location = vacancy->brief.location;

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
        qCDebug(LinkedApp) << "Sorting by relevance";

        auto compareByRelevance = [&](const Vacancy* left, const Vacancy* right)
        {
            return left->count(keywords) > right->count(keywords);
        };

        std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareByRelevance);
    }

    void App::sortByTitle()
    {
        qCDebug(LinkedApp) << "Sorting by title";

        auto compareByTitle = [](const Vacancy* left, const Vacancy* right)
        {
            return left->brief.title < right->brief.title;
        };

        std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareByTitle);
    }

    void App::sortByCompany()
    {
        qCDebug(LinkedApp) << "Sorting by company";

        auto compareByCompany = [](const Vacancy* left, const Vacancy* right)
        {
            return left->brief.companyName < right->brief.companyName;
        };

        std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareByCompany);
    }

    void App::sortByTime()
    {
        qCDebug(LinkedApp) << "Sorting by time";

        auto compareByTime = [](const Vacancy* left, const Vacancy* right)
        {
            return left->brief.date > right->brief.date;
        };

        std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareByTime);
    }

    void App::sortByEmploymentType()
    {
        qCDebug(LinkedApp) << "sorting by Relevance";
        auto compareByEmploymentType = [](const Vacancy* left, const Vacancy* right)
        {
            return left->details.employmentType > right->details.employmentType;
        };

        std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareByEmploymentType);
    }

    void App::sortByLevel()
    {
        qCDebug(LinkedApp) << "Sorting by level";

        auto compareByLevel = [&](const Vacancy* left, const Vacancy* right)
        {
            if (!left->details.level.has_value()) return false;
            if (!right->details.level.has_value()) return true;

            return left->details.level < right->details.level;
        };

        std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareByLevel);
    }

    void App::sortByExperience()
    {
        qCDebug(LinkedApp) << "Sorting by experience";

        auto compareByExperience = [&](const Vacancy* left, const Vacancy* right)
        {
            if (!left->details.experience.has_value()) return false;
            if (!right->details.experience.has_value()) return true;

            return left->details.experience < right->details.experience;
        };

        std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareByExperience);
    }

    void App::sortByEducation()
    {
        qCDebug(LinkedApp) << "Sorting by education";

        auto compareByEducation = [&](const Vacancy* left, const Vacancy* right)
        {
            if (!left->details.education.has_value()) return false;
            if (!right->details.education.has_value()) return true;

            return left->details.education < right->details.education;
        };

        std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareByEducation);
    }

    void App::sortBySkills()
    {
        qCDebug(LinkedApp) << "Sorting by skills";

        auto compareBySkills = [&](const Vacancy* left, const Vacancy* right)
        {
            if (left->details.skills.isEmpty()) return false;
            if (right->details.skills.isEmpty()) return true;

            return left->details.skills < right->details.skills;
        };

        std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareBySkills);
    }

    void App::sortByIndustry()
    {
        qCDebug(LinkedApp) << "sorting by industry";

        auto compareByIndustry = [&](const Vacancy* left, const Vacancy* right)
        {
            if (!left->details.industry.has_value()) return false;
            if (!right->details.industry.has_value()) return true;

            return left->details.industry < right->details.industry;
        };

        std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareByIndustry);
    }

    void App::sortByExpirationDate()
    {
        qCDebug(LinkedApp) << "sorting by Relevance";

        auto compareByExpirationDate = [&](const Vacancy* left, const Vacancy* right)
        {
            if (!left->details.expirationDate.has_value()) return false;
            if (!right->details.expirationDate.has_value()) return true;

            return left->details.expirationDate < right->details.expirationDate;
        };

        std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareByExpirationDate);
    }

    std::optional<App::constVacancyRef> App::getCurrentVacancy() const
    {
        const QModelIndex& index = gui->mainWindow()->vacanciesList.currentIndex();

        if (!index.isValid())
            return std::nullopt;

        const int id = index.row();

        return *vacanciesToShow[id];
    }

    VacancyStatus App::getVacancyStatus(const Vacancy& vacancy)
    {
        const QUrl& url = vacancy.brief.link;

        if (interestedVacancies.contains(url))
            return VacancyStatus::Interested;

        if (ignoredVacancies.contains(url))
            return VacancyStatus::Ignored;

        return VacancyStatus::Unreviewed;
    }

    void App::saveState()
    {
        const MainWindow& mainWindow = *gui->mainWindow();

        state.setIgnored(ignoredVacancies);
        state.setInterested(interestedVacancies);

        state.setKeywords(mainWindow.keywordsLineEdit.text());
        state.setLocation(mainWindow.locationLineEdit.text());

        state.setLimit(mainWindow.limitVacanciesInput.value());

        state.setFilterMode(mainWindow.filterMode.currentIndex());
        state.setFilters(mainWindow.filterLineEdit.text());

        state.setSortMode(mainWindow.sortMode.currentIndex());

        state.setIsUnreviewedVisible(mainWindow.unreviewedCheckBox.isChecked());
        state.setIsIgnoredVisible(mainWindow.ignoredCheckBox.isChecked());
        state.setIsInterestedVisible(mainWindow.interestedCheckBox.isChecked());


        state.saveState();
    }

    void App::loadState()
    {
        state.loadState();

        MainWindow& mainWindow = *gui->mainWindow();

        ignoredVacancies = state.getIgnored();
        interestedVacancies = state.getInterested();

        mainWindow.keywordsLineEdit.setText(state.getKeywords());
        mainWindow.locationLineEdit.setText(state.getLocation());

        mainWindow.limitVacanciesInput.setValue(state.getLimit());

        mainWindow.filterMode.setCurrentIndex(state.getFilterMode());
        mainWindow.filterLineEdit.setText(state.getFilters());

        mainWindow.sortMode.setCurrentIndex(state.getSortMode());


        mainWindow.unreviewedCheckBox.setChecked(state.getIsUnreviewedVisible());
        mainWindow.ignoredCheckBox.setChecked(state.getIsIgnoredVisible());
        mainWindow.interestedCheckBox.setChecked(state.getIsInterestedVisible());
    }

} // namespace Linked