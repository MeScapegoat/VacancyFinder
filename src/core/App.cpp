// std

// Qt
#include <QCoreApplication>
#include <QFile>
#include <QUrlQuery>
#include <QDir>
#include <QSpinBox>

// custom
#include "App.h"
#include "../network/RequestManager.h"

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

        QObject::connect(networkThread, &QThread::finished, &requestManager, &RequestManager::deleteLater);
        QObject::connect(parserThread,  &QThread::finished, parser,         &HTMLparser::deleteLater);

        QObject::connect(&gui->mainWindow()->limitVacanciesInput, &QSpinBox::valueChanged, this, &App::updateMaxVacancies);

        QObject::connect(&requestManager, &RequestManager::requestsFinished, this, &App::onRequestsFinished);
        QObject::connect(&requestManager, &RequestManager::requestsFinished, parser, &HTMLparser::onRequestsFinished);

        QObject::connect(&requestManager, &RequestManager::receivedReply, parser, &HTMLparser::processHTML);
        QObject::connect(this, &App::initSearch, &requestManager, &RequestManager::addUrls);

        QObject::connect(parser, &HTMLparser::sendRequest, &requestManager, &RequestManager::addUrl);

        QObject::connect(parser, &HTMLparser::briefsFinished,     this, &App::onBriefsFinished);
        QObject::connect(parser, &HTMLparser::allDetailsFinished, this, &App::onAllDetailsFinished);

        QObject::connect(parser, &HTMLparser::briefReady,   this, &App::onBriefReady);
        QObject::connect(parser, &HTMLparser::detailsReady, this, &App::onDetailsReady);

        QObject::connect(parser, &HTMLparser::briefReady,   gui->progressDialog(), &ProgressDialog::updateBrief);
        QObject::connect(parser, &HTMLparser::detailsReady, gui->progressDialog(), &ProgressDialog::updateDetails);

        QObject::connect(this, &App::updateVacanciesList, gui->mainWindow(), &MainWindow::updateVacanciesList);
        QObject::connect(gui->mainWindow()->list(), &QListView::clicked, this, &App::openVacancyDescription);
        QObject::connect(gui->mainWindow()->button(), &QPushButton::released, this, &App::sendSearchRequest);

        QObject::connect(this, &App::openProgressDialog,  gui->progressDialog(), &ProgressDialog::start);
        QObject::connect(this, &App::closeProgressDialog, gui->progressDialog(), &ProgressDialog::accept);

        QObject::connect(gui->mainWindow()->sortMode(), &QButtonGroup::idClicked, this, &App::updateDescriptionList);
        QObject::connect(gui->mainWindow()->filterMode(), &QButtonGroup::idClicked, this, &App::updateDescriptionList);

        QObject::connect(&requestManager, &RequestManager::replyError, gui->progressDialog(), &ProgressDialog::updateError);

        QObject::connect(&gui->progressDialog()->stopButton, &QPushButton::released, &requestManager, &RequestManager::onInterrupt);
        QObject::connect(&gui->progressDialog()->stopButton, &QPushButton::released, parser,         &HTMLparser::onInterrupt);

        networkThread->start();
        parserThread->start();

        context->app = this;
        context->mainWindow = gui->mainWindow();
    }

    App::~App()
    {
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

        keywords = gui->mainWindow()->keywords()->text();
        location = gui->mainWindow()->location()->text();

        qCInfo(LinkedApp) << "keywords:" << keywords << "location:" << location;

        QVector<QUrl> urls;

        if (maxVacancies > 10)
        {
            for (auto n = 0; n < maxVacancies; n += 10)
            {
                QUrl url(baseUrl);
                QUrlQuery query;
                query.addQueryItem("keywords", QUrl::toPercentEncoding(keywords));
                query.addQueryItem("location", QUrl::toPercentEncoding(location));
                query.addQueryItem("start", QString::number(n));
                url.setQuery(query);
        
                urls.append(url);
            }
        }
        
        QUrl url(baseUrl);
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

        const QString& title = vacancy.brief.title;
        const QString& description = vacancy.details.description;
        const QString& level = vacancy.details.level.value_or("No data");
        const QString& employmentType = vacancy.details.employmentType;
        const QString& companyName = vacancy.brief.companyName;
        const QString& companyLink = vacancy.brief.companyLink.toString();

        qCInfo(LinkedApp) << "Loading descrption:" << id << "." << title;

        const QString HTMLformated =  "<h1>Description:</h1>" + description +
                                "<h1>Level:</h1>" + level +
                                "<h1>employmentType:</h1>" + employmentType;

        //QString HTMLdebug = "<h1>description</h1>" + vacancy.description +
        //    "<h1>title</h1>" + vacancy.title +
        //    "<h1>link</h1>" + vacancy.link.toString() +
        //    "<h1>companyName</h1>" + vacancy.companyName +
        //    "<h1>companyLink</h1>" + vacancy.companyLink.toString() +
        //    "<h1>date</h1>" + vacancy.date.toString() +
        //    "<h1>location</h1>" + vacancy.location +
        //    "<h1>level</h1>" + vacancy.level +
        //    "<h1>employmentType</h1>" + vacancy.employmentType;

        QString linkPattern = "<a href='%1'>%2</a>";

        gui->mainWindow()->titleLabel()->setText(linkPattern.arg(vacancy.brief.link.toString()).arg(title));
        gui->mainWindow()->companyNameLabel()->setText(linkPattern.arg(companyLink).arg(companyName));
        gui->mainWindow()->levelLabel()->setText(level);

        gui->mainWindow()->descr()->setText(description);
    }

    void App::updateDescriptionList(int temp)
    {
        gui->mainWindow()->clearList();

        auto sortMode = gui->mainWindow()->sortMode()->checkedId();
        qCDebug(LinkedApp) << "processing sort mode:" << sortMode;

        switch (sortMode)
        {
            case 1:
            sortByRelevance(keywords);
            break;
            case 2:
            sortByTime();
            break;
            case 3:
            sortByTitle();
            break;
            case 4:
            sortByCompany();
            break;
            case 5:
            sortByLevel();
            break;
            default:
            qCCritical(LinkedApp) << "Checked sort mode is incorrect! Sort mode:" << sortMode;
            break;
        }

        auto filterMode = gui->mainWindow()->filterMode()->checkedId();
        qCDebug(LinkedApp) << "processing filter mode:" << filterMode;

        const QString& filterText = gui->mainWindow()->filterLineEdit()->text();
        auto filterWords = filterText.split(',');
        for (auto& word : filterWords)
            word = word.trimmed();
        qCDebug(LinkedApp) << "filter words:" << filterWords;

        switch (filterMode)
        {
            case 0:
            break;
            case 1: // MUST include
            for (auto it = vacanciesToShow.begin(); it != vacanciesToShow.end();)
            {
                const auto& vacancy = **it;
                bool isFilterOk = true;
                for (const auto& word : filterWords)
                {
                    if (!vacancy.brief.title.contains(word) && !vacancy.details.description.contains(word))
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
            case 2: // MUST NOT include
            for (auto it = vacanciesToShow.begin(); it != vacanciesToShow.end();)
            {
                const auto& vacancy = **it;
                bool isFilterOk = true;
                for (const auto& word : filterWords)
                {
                    if (vacancy.brief.title.contains(word) || vacancy.details.description.contains(word))
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
            default:
            qCCritical(LinkedApp) << "Checked filter mode is incorrect! Filter mode:" << filterMode;
            break;
        }

        emit updateVacanciesList();
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

    void App::sortByTime()
    {
        auto compareByTime = [](const auto& left, const auto& right)
        {
            return left->brief.date > right->brief.date;
        };

        syncVacanciesToShow();

        std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareByTime);
    }

    void App::sortByTitle()
    {
        auto compareByTitle = [](const auto& left, const auto& right)
        {
            return left->brief.title < right->brief.title;
        };

        syncVacanciesToShow();

        std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareByTitle);
    }

    void App::sortByCompany()
    {
        auto compareByCompany = [](const auto& left, const auto& right)
        {
            return left->brief.companyName < right->brief.companyName;
        };

        syncVacanciesToShow();

        std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareByCompany);
    }

    void App::sortByRelevance(const QString& str)
    {
        auto compareByRelevance = [&](const auto& left, const auto& right)
        {
            return left->count(str) > right->count(str);
        };

        syncVacanciesToShow();

        std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareByRelevance);
    }

    void App::sortByLevel()
    {
        auto compareByLevel = [&](const auto& left, const auto& right)
        {
            return left->details.level < right->details.level;
        };

        syncVacanciesToShow();

        std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareByLevel);
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

    const unsigned& App::getMaxVacancies() const
    {
        return maxVacancies;
    }

} // namespace Linked