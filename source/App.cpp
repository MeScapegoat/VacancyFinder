// std

// Qt
#include <QCoreApplication>
#include <QFile>
#include <QUrlQuery>
#include <QDir>

// custom
#include "App.h"

namespace Linked
{
    Q_LOGGING_CATEGORY(LinkedApp, "app");

    App::App()
    {
        networkThread.setObjectName("networkThread");
        networkManager.moveToThread(&networkThread);

        parserThread.setObjectName("parserThread");
        parser.moveToThread(&parserThread);

        guiThread.setObjectName("guiThread");
        gui.moveToThread(&guiThread);

        QObject::connect(&networkManager, &NetworkManager::finished, &parser, &HTMLparser::processHTML, Qt::QueuedConnection);
        QObject::connect(this, &App::sendRequest, &networkManager, &NetworkManager::sendRequest, Qt::QueuedConnection);

        QObject::connect(&parser, &HTMLparser::sendRequest, &networkManager, &NetworkManager::sendRequest, Qt::QueuedConnection);
        QObject::connect(&parser, &HTMLparser::titlesProcessed, this, &App::titlesProcessed, Qt::DirectConnection);
        QObject::connect(&parser, &HTMLparser::requestFinished, this, &App::requestFinished, Qt::DirectConnection);
        QObject::connect(&parser, &HTMLparser::titleReady, this, &App::titleReady, Qt::DirectConnection);
        QObject::connect(&parser, &HTMLparser::descrReady, this, &App::descrReady, Qt::DirectConnection);

        QObject::connect(this, &App::toList, gui.mainWindow(), &MainWindow::appendList, Qt::DirectConnection);
        QObject::connect(gui.mainWindow()->list(), &QListView::clicked, this, &App::openVacancyDescription, Qt::QueuedConnection);
        QObject::connect(gui.mainWindow()->button(), &QPushButton::released, this, &App::sendSearchRequest, Qt::QueuedConnection);

        QObject::connect(this, &App::openSearchMessage, gui.inSearchMessage(), &QMessageBox::exec, Qt::QueuedConnection);
        QObject::connect(this, &App::closeSearchMessage, gui.inSearchMessage(), &QMessageBox::accept, Qt::QueuedConnection);

        QObject::connect(this, &App::openSearchMessage, gui.mainWindow(), &MainWindow::clearList, Qt::QueuedConnection);

        networkThread.start();
        parserThread.start();
        guiThread.start();
    }

    App::~App()
    {
        networkThread.quit();
        networkThread.wait();

        parserThread.quit();
        parserThread.wait();

        guiThread.quit();
        guiThread.wait();
    }

    void App::sendSearchRequest()
    {
        vacancies.clear();
        descrProcessed = 0;

        keywords = gui.mainWindow()->keywords()->text();
        location = gui.mainWindow()->location()->text();

        qCInfo(LinkedApp) << "keywords:" << keywords << "location:" << location;

        QUrl url(baseUrl);
        QUrlQuery query;
        query.addQueryItem("keywords", QUrl::toPercentEncoding(keywords));
        query.addQueryItem("location", QUrl::toPercentEncoding(location));
        query.addQueryItem("start", QUrl::toPercentEncoding("0"));
        url.setQuery(query);
        searchStartTime = QTime::currentTime();
        emit openSearchMessage();
        emit sendRequest(url);
    }

    void App::openVacancyDescription(const QModelIndex &index)
    {
        auto id = index.row();
        if (id > vacancies.size())
        {
            qCDebug(LinkedApp) << "Not ready yet!";
            return;
        }
        qCInfo(LinkedApp) << "Loading descrption:" << id << "." << vacancies[id].title;
        QString HTMLformated = "<h1>Description:</h1>" + vacancies[id].description +
                               "<h1>Level:</h1>" + vacancies[id].level +
                               "<h1>employmentType:</h1>" + vacancies[id].employmentType;

        QString HTMLdebug = "<h1>description</h1>" + vacancies[id].description +
                            "<h1>title</h1>" + vacancies[id].title +
                            "<h1>link</h1>" + vacancies[id].link +
                            "<h1>companyName</h1>" + vacancies[id].companyName +
                            "<h1>companyLink</h1>" + vacancies[id].companyLink +
                            "<h1>date</h1>" + vacancies[id].date.toString() +
                            "<h1>location</h1>" + vacancies[id].location +
                            "<h1>level</h1>" + vacancies[id].level +
                            "<h1>employmentType</h1>" + vacancies[id].employmentType;
        gui.mainWindow()->descr()->setText(HTMLdebug); // vacancies[id].description
    }

    void App::titleReady(const Vacancy &vacancy)
    {
        qCInfo(LinkedApp) << vacancies.size() << "titleReady:";
        qCDebug(LinkedApp) << "title:" << vacancy.title;
        qCDebug(LinkedApp) << "companyName:" << vacancy.companyName;
        qCDebug(LinkedApp) << "location:" << vacancy.location;
        qCDebug(LinkedApp) << "date:" << vacancy.date;
        qCDebug(LinkedApp) << "link:" << vacancy.link;
        qCDebug(LinkedApp) << "companyLink:" << vacancy.companyLink;
        vacancies.push_back(vacancy);
    }

    void App::descrReady(const Vacancy &vacancy)
    {
        qCInfo(LinkedApp) << descrProcessed << "Description Ready";
        qCDebug(LinkedApp) << "description[0, 50]:" << vacancy.description.mid(0, 50);
        qCDebug(LinkedApp) << "level:" << vacancy.level;
        qCDebug(LinkedApp) << "employmentType:" << vacancy.employmentType;

        if (vacancies.size() <= descrProcessed)
        {
            qCCritical(LinkedApp) << "there are less vacancies than descriptions provided";
            return;
        }

        Vacancy &vac = vacancies[descrProcessed];
        vac.description = vacancy.description;
        vac.level = vacancy.level;
        vac.employmentType = vacancy.employmentType;
        vac.HTML = vacancy.HTML;

        descrProcessed++;
        // emit toList(vacancies[descrProcessed]);

        if (descrProcessed >= 999)
        {
            requestFinished();
            return;
        }

        if (descrProcessed == vacancies.size())
        {
            QUrl url(baseUrl);
            QUrlQuery query;
            query.addQueryItem("keywords", QUrl::toPercentEncoding(keywords));
            query.addQueryItem("location", QUrl::toPercentEncoding(location));
            query.addQueryItem("start", QUrl::toPercentEncoding(QString::number(vacancies.size())));
            url.setQuery(query);
            emit sendRequest(url);
        }
        else
        {
            emit sendRequest(vacancies[descrProcessed].link);
        }
    }

    void App::titlesProcessed()
    {
        qCInfo(LinkedApp) << "vacancies:" << vacancies.size();
        emit sendRequest(vacancies[descrProcessed].link);
    }

    void App::requestFinished()
    {
        qCInfo(LinkedApp) << "Time spent for" << vacancies.size() << "vacancies is" << searchStartTime.secsTo(QTime::currentTime()) << "seconds";

        switch (gui.mainWindow()->sortMode()->checkedId())
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
        case -1:
            qCCritical(LinkedApp) << "No sort mode is checked!";
            break;
        default:
            qCCritical(LinkedApp) << "Checked sort mode is incorrect! Sort mode:" << gui.mainWindow()->sortMode()->checkedId();
            break;
        }

        for (const auto &vacancy : vacancies)
            toList(vacancy);

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

    QString App::loadTitleFromFile(const QString &path)
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

    QString App::loadDescriptionFromFile(const QString &path)
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

    const QList<Vacancy> &App::sortByTime()
    {
        auto compareByTime = [](const auto &left, const auto &right)
        {
            return left.date > right.date;
        };
        std::sort(vacancies.begin(), vacancies.end(), compareByTime);
        return vacancies;
    }

    const QList<Vacancy> &App::sortByTitle()
    {
        auto compareByTitle = [](const auto &left, const auto &right)
        {
            return left.title < right.title;
        };
        std::sort(vacancies.begin(), vacancies.end(), compareByTitle);
        return vacancies;
    }

    const QList<Vacancy> &App::sortByCompany()
    {
        auto compareByCompany = [](const auto &left, const auto &right)
        {
            return left.companyName < right.companyName;
        };

        std::sort(vacancies.begin(), vacancies.end(), compareByCompany);
        return vacancies;
    }

    const QList<Vacancy> &App::sortByRelevance(const QString &str)
    {
        auto compareByRelevance = [&](const auto &left, const auto &right)
        {
            return left.count(str) > right.count(str);
        };

        std::sort(vacancies.begin(), vacancies.end(), compareByRelevance);
        return vacancies;
    }

    const QList<Vacancy> &App::sortByLevel()
    {
        auto compareByRelevance = [&](const auto &left, const auto &right)
        {
            return left.level < right.level;
        };

        std::sort(vacancies.begin(), vacancies.end(), compareByRelevance);
        return vacancies;
    }

    const QList<Vacancy> &App::getVacancies()
    {
        return vacancies;
    }

} // namespace Linked