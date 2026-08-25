// std

// Qt
#include <QCoreApplication>
#include <QFile>
#include <QUrlQuery>
#include <QDir>

// custom
#include "App.h"
#include "..\network\NetworkManager.h"

namespace Linked
{
    Q_LOGGING_CATEGORY(LinkedApp, "app");

    App::App(QObject* parent) :
        QObject(parent),
        context(new SharedContext),
        networkManager(new NetworkManager(context)), networkThread(new QThread(this)),
        parser(new HTMLparser(context)), parserThread(new QThread(this)),
        gui(new GUI(context))
    {
        networkThread->setObjectName("networkThread");
        networkManager->moveToThread(networkThread);

        parserThread->setObjectName("parserThread");
        parser->moveToThread(parserThread);

        QObject::connect(networkManager, &NetworkManager::receivedReply, parser, &HTMLparser::processHTML, Qt::QueuedConnection);
        QObject::connect(this, &App::sendRequest, networkManager, &NetworkManager::addUrl, Qt::QueuedConnection);

        QObject::connect(parser, &HTMLparser::sendRequest, networkManager, &NetworkManager::addUrl, Qt::QueuedConnection);
        QObject::connect(parser, &HTMLparser::titlesProcessed, this, &App::titlesProcessed, Qt::DirectConnection);
        QObject::connect(parser, &HTMLparser::requestFinished, this, &App::requestFinished, Qt::DirectConnection);
        QObject::connect(parser, &HTMLparser::titleReady, this, &App::titleReady, Qt::DirectConnection);
        QObject::connect(parser, &HTMLparser::descrReady, this, &App::descrReady, Qt::DirectConnection);

        QObject::connect(this, &App::updateVacanciesList, gui->mainWindow(), &MainWindow::updateVacanciesList, Qt::DirectConnection);
        QObject::connect(gui->mainWindow()->list(), &QListView::clicked, this, &App::openVacancyDescription, Qt::QueuedConnection);
        QObject::connect(gui->mainWindow()->button(), &QPushButton::released, this, &App::sendSearchRequest, Qt::QueuedConnection);

        QObject::connect(this, &App::openProgressDialog, gui->progressDialog(), &ProgressDialog::start, Qt::QueuedConnection);
        QObject::connect(this, &App::closeSearchMessage, gui->progressDialog(), &ProgressDialog::accept, Qt::QueuedConnection);

        QObject::connect(this, &App::openProgressDialog, gui->mainWindow(), &MainWindow::clearList, Qt::QueuedConnection);

        QObject::connect(gui->mainWindow()->sortMode(), &QButtonGroup::idClicked, this, &App::updateDescriptionList, Qt::QueuedConnection);
        QObject::connect(gui->mainWindow()->filterMode(), &QButtonGroup::idClicked, this, &App::updateDescriptionList, Qt::QueuedConnection);

        QObject::connect(parser, &HTMLparser::descrStart, gui->progressDialog(), &ProgressDialog::updateInfo, Qt::QueuedConnection);
        QObject::connect(parser, &HTMLparser::replyError, gui->progressDialog(), &ProgressDialog::updateError, Qt::QueuedConnection);
        QObject::connect(gui->progressDialog()->stopButton(), &QPushButton::released, this, &App::requestFinished, Qt::DirectConnection);

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
        descrProcessed = 0;
        isRequestFinished = false;

        keywords = gui->mainWindow()->keywords()->text();
        location = gui->mainWindow()->location()->text();

        qCInfo(LinkedApp) << "keywords:" << keywords << "location:" << location;

        QUrl url(baseUrl);
        QUrlQuery query;
        query.addQueryItem("keywords", QUrl::toPercentEncoding(keywords));
        query.addQueryItem("location", QUrl::toPercentEncoding(location));
        query.addQueryItem("start", QUrl::toPercentEncoding("0"));
        url.setQuery(query);
        searchStartTime = QTime::currentTime();
        emit openProgressDialog();
        emit sendRequest(url);
    }

    void App::openVacancyDescription(const QModelIndex& index)
    {
        auto id = index.row();
        if (id > vacanciesToShow.size())
        {
            qCDebug(LinkedApp) << "Not ready yet!";
            return;
        }
        const Vacancy& vacancy = *vacanciesToShow[id];
        qCInfo(LinkedApp) << "Loading descrption:" << id << "." << vacancy.title;
        QString HTMLformated = "<h1>Description:</h1>" + vacancy.description +
            "<h1>Level:</h1>" + vacancy.level +
            "<h1>employmentType:</h1>" + vacancy.employmentType;

        QString HTMLdebug = "<h1>description</h1>" + vacancy.description +
            "<h1>title</h1>" + vacancy.title +
            "<h1>link</h1>" + vacancy.link.toString() +
            "<h1>companyName</h1>" + vacancy.companyName +
            "<h1>companyLink</h1>" + vacancy.companyLink.toString() +
            "<h1>date</h1>" + vacancy.date.toString() +
            "<h1>location</h1>" + vacancy.location +
            "<h1>level</h1>" + vacancy.level +
            "<h1>employmentType</h1>" + vacancy.employmentType;

        QString linkPattern = "<a href='%1'>%2</a>";

        gui->mainWindow()->titleLabel()->setText(linkPattern.arg(vacancy.link.toString()).arg(vacancy.title));
        gui->mainWindow()->companyNameLabel()->setText(linkPattern.arg(vacancy.companyLink.toString()).arg(vacancy.companyName));
        gui->mainWindow()->levelLabel()->setText(vacancy.level);

        gui->mainWindow()->descr()->setText(vacancy.description);
    }

    void App::titleReady(const Vacancy& vacancy)
    {
        if (isRequestFinished)
        {
            qCDebug(LinkedApp) << "request finished: quit titleReady";
            return;
        }

        if (vacancies.size() >= context->mainWindow->maxVacancies())
            return;

        qCInfo(LinkedApp) << vacancies.size() << "titleReady:";
        qCDebug(LinkedApp) << "title:       " << vacancy.title;
        qCDebug(LinkedApp) << "link:        " << vacancy.link;
        qCDebug(LinkedApp) << "companyName: " << vacancy.companyName;
        qCDebug(LinkedApp) << "companyLink: " << vacancy.companyLink;
        qCDebug(LinkedApp) << "location:    " << vacancy.location;
        qCDebug(LinkedApp) << "date:        " << vacancy.date;

        updateVacancies(vacancy);
    }

    void App::descrReady(const Vacancy& vacancy)
    {
        if (isRequestFinished)
        {
            qCDebug(LinkedApp) << "request finished: quit descrReady";
            return;
        }
        qCInfo(LinkedApp) << descrProcessed << "Description Ready";
        qCDebug(LinkedApp) << "description[0, 50]:" << vacancy.description.mid(0, 50);
        qCDebug(LinkedApp) << "level:" << vacancy.level;
        qCDebug(LinkedApp) << "employmentType:" << vacancy.employmentType;

        if (vacancies.size() <= descrProcessed)
        {
            qCCritical(LinkedApp) << "there are less vacancies than descriptions provided";
            return;
        }

        updateVacancies(vacancy);

        //Vacancy& vac = vacancies[descrProcessed];
        //vac.description = vacancy.description;
        //vac.level = vacancy.level;
        //vac.employmentType = vacancy.employmentType;

        descrProcessed++;

        if (descrProcessed >= context->mainWindow->maxVacancies())
        {
            requestFinished();
            return;
        }

        //if (descrProcessed == vacancies.size())
        //{
            QUrl url(baseUrl);
            QUrlQuery query;
            query.addQueryItem("keywords", QUrl::toPercentEncoding(keywords));
            query.addQueryItem("location", QUrl::toPercentEncoding(location));
            query.addQueryItem("start", QUrl::toPercentEncoding(QString::number(vacancies.size())));
            url.setQuery(query);
            emit sendRequest(url);
        //}
        //else
        //{
        //    //emit sendRequest(vacancies[descrProcessed].link);
        //}
    }

    void App::titlesProcessed()
    {
        qCInfo(LinkedApp) << "vacancies:" << vacancies.size();
        //emit sendRequest(vacancies.at(descrProcessed).link);
    }

    void App::requestFinished()
    {
        qCDebug(LinkedApp) << "request finished";
        isRequestFinished = true;
        //vacancies.resize(descrProcessed);
        qCInfo(LinkedApp) << "Time spent for" << vacancies.size() << "vacancies is" << searchStartTime.secsTo(QTime::currentTime()) << "seconds";

        updateDescriptionList();

        // QDir sortedDir("sorted");
        // if (!sortedDir.exists())
        //     sortedDir.mkpath(".");

        // sortByRelevance(keywords);

        // QFile relevanceSorted(sortedDir.absoluteFilePath("relevanceSorted"));
        // if (relevanceSorted.open(QIODeviceBase::WriteOnly))
        // {
        //     QTextStream relevanceSortedStream(&relevanceSorted);
        //     for (const auto &vacancy : vacancies)
        //     {
        //         vacancy.print(relevanceSortedStream);
        //         relevanceSortedStream << "\n";
        //     }
        //     relevanceSorted.close();
        // }

        // sortByTime();

        // QFile timeSorted(sortedDir.absoluteFilePath("timeSorted"));
        // if (timeSorted.open(QIODeviceBase::WriteOnly))
        // {
        //     QTextStream timeSortedStream(&timeSorted);
        //     for (const auto &vacancy : vacancies)
        //     {
        //         vacancy.print(timeSortedStream);
        //         timeSortedStream << "\n";
        //     }
        //     timeSorted.close();
        // }

        // sortByTitle();

        // QFile titleSorted(sortedDir.absoluteFilePath("titleSorted"));
        // if (titleSorted.open(QIODeviceBase::WriteOnly))
        // {
        //     QTextStream titleSortedStream(&titleSorted);
        //     for (const auto &vacancy : vacancies)
        //     {
        //         vacancy.print(titleSortedStream);
        //         titleSortedStream << "\n";
        //     }
        //     titleSorted.close();
        // }

        // sortByCompany();

        // QFile companySorted(sortedDir.absoluteFilePath("companySorted"));
        // if (companySorted.open(QIODeviceBase::WriteOnly))
        // {
        //     QTextStream companySortedStream(&companySorted);
        //     for (const auto &vacancy : vacancies)
        //     {
        //         vacancy.print(companySortedStream);
        //         companySortedStream << "\n";
        //     }
        //     companySorted.close();
        // }

        // sortByLevel();

        // QFile levelSorted(sortedDir.absoluteFilePath("levelSorted"));
        // if (levelSorted.open(QIODeviceBase::WriteOnly))
        // {
        //     QTextStream levelSortedStream(&levelSorted);
        //     for (const auto &vacancy : vacancies)
        //     {
        //         vacancy.print(levelSortedStream);
        //         levelSortedStream << "\n";
        //     }
        //     levelSorted.close();
        // }
        emit closeSearchMessage();
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
                    if (!vacancy.title.contains(word) && !vacancy.description.contains(word))
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
                    if (vacancy.title.contains(word) || vacancy.description.contains(word))
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
            return left->date > right->date;
        };

        syncVacanciesToShow();

        std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareByTime);
    }

    void App::sortByTitle()
    {
        auto compareByTitle = [](const auto& left, const auto& right)
        {
            return left->title < right->title;
        };

        syncVacanciesToShow();

        std::sort(vacanciesToShow.begin(), vacanciesToShow.end(), compareByTitle);
    }

    void App::sortByCompany()
    {
        auto compareByCompany = [](const auto& left, const auto& right)
        {
            return left->companyName < right->companyName;
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
            return left->level < right->level;
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

    void App::updateVacancies(const Vacancy& vacancy)
    {
        auto it = vacancies.find(vacancy.link);
        if (it == vacancies.end())
        {
            qCDebug(LinkedApp) << "NO VACANCY WITH SUCH LINK";
            vacancies.insert(vacancy.link, vacancy);
        }
        else
        {
            qCDebug(LinkedApp) << "THERE IS VACANCY WITH SUCH LINK";
            Vacancy& foundVacancy = it.value();
            foundVacancy.mergeFrom(vacancy);
        }
    }

    void App::syncVacanciesToShow()
    {
        vacanciesToShow.clear();
        for (const Vacancy& vacancy : vacancies)
        {
            vacanciesToShow.push_back(&vacancy);
        }
    }

} // namespace Linked