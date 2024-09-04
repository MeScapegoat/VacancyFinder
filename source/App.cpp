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

        keyword = gui.mainWindow()->keywords()->text();
        location = gui.mainWindow()->location()->text();

        qCInfo(LinkedApp) << "keyword:" << keyword << "location:" << location;

        QUrl url(baseUrl);
        QUrlQuery query;
        query.addQueryItem("keyword", QUrl::toPercentEncoding(keyword));
        query.addQueryItem("location", QUrl::toPercentEncoding(location));
        query.addQueryItem("start", QUrl::toPercentEncoding("0"));
        url.setQuery(query);
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
        gui.mainWindow()->descr()->setText("Description:\n" + vacancies[id].description + "\nLevel:\n" + vacancies[id].level + "\nemploymentType:\n" + vacancies[id].employmentType);
    }

    void App::titleReady(const Vacancy &vacancy)
    {
        vacancies.push_back(vacancy);
    }

    void App::descrReady(const Vacancy &vacancy)
    {
        qCDebug(LinkedApp) << "Processing description:" << descrProcessed + 1;
        if (vacancies.size() < descrProcessed + 1)
        {
            qCCritical(LinkedApp) << "there are less vacancies than descriptions provided";
            return;
        }
        Vacancy &vac = vacancies[descrProcessed++];
        vac.description = vacancy.description;
        vac.level = vacancy.level;
        vac.employmentType = vacancy.employmentType;
        vac.HTML = vacancy.HTML;

        if (descrProcessed == vacancies.size())
        {
            if (vacancies.size() > 999)
            {
                requestFinished();
                return;
            }

            QUrl url(baseUrl);
            QUrlQuery query;
            query.addQueryItem("keyword", QUrl::toPercentEncoding(keyword));
            query.addQueryItem("location", QUrl::toPercentEncoding(location));
            query.addQueryItem("start", QUrl::toPercentEncoding(QString::number(vacancies.size())));
            url.setQuery(query);
            emit sendRequest(url);
            return;
        }

        emit toList(vacancies[descrProcessed]);
        emit sendRequest(vacancies[descrProcessed].link);
    }

    void App::titlesProcessed()
    {
        qCInfo(LinkedApp) << "vacancies:" << vacancies.size();
        emit sendRequest(vacancies[descrProcessed].link);
    }

    void App::requestFinished()
    {
        qCDebug(LinkedApp);

        QDir sortedDir("sorted");
        if (!sortedDir.exists())
            sortedDir.mkpath(".");

        sortByRelevance(keyword);

        QFile relevanceSorted(sortedDir.absoluteFilePath("relevanceSorted"));
        if (relevanceSorted.open(QIODeviceBase::WriteOnly))
        {
            QTextStream relevanceSortedStream(&relevanceSorted);
            for (const auto &vacancy : vacancies)
            {
                vacancy.print(relevanceSortedStream);
                relevanceSortedStream << "\n";
            }
            relevanceSorted.close();
        }

        sortByTime();

        QFile timeSorted(sortedDir.absoluteFilePath("timeSorted"));
        if (timeSorted.open(QIODeviceBase::WriteOnly))
        {
            QTextStream timeSortedStream(&timeSorted);
            for (const auto &vacancy : vacancies)
            {
                vacancy.print(timeSortedStream);
                timeSortedStream << "\n";
            }
            timeSorted.close();
        }

        sortByTitle();

        QFile titleSorted(sortedDir.absoluteFilePath("titleSorted"));
        if (titleSorted.open(QIODeviceBase::WriteOnly))
        {
            QTextStream titleSortedStream(&titleSorted);
            for (const auto &vacancy : vacancies)
            {
                vacancy.print(titleSortedStream);
                titleSortedStream << "\n";
            }
            titleSorted.close();
        }

        sortByCompany();

        QFile companySorted(sortedDir.absoluteFilePath("companySorted"));
        if (companySorted.open(QIODeviceBase::WriteOnly))
        {
            QTextStream companySortedStream(&companySorted);
            for (const auto &vacancy : vacancies)
            {
                vacancy.print(companySortedStream);
                companySortedStream << "\n";
            }
            companySorted.close();
        }

        sortByLevel();

        QFile levelSorted(sortedDir.absoluteFilePath("levelSorted"));
        if (levelSorted.open(QIODeviceBase::WriteOnly))
        {
            QTextStream levelSortedStream(&levelSorted);
            for (const auto &vacancy : vacancies)
            {
                vacancy.print(levelSortedStream);
                levelSortedStream << "\n";
            }
            levelSorted.close();
        }
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