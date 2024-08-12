// std

// Qt
#include <QThread>

// custom
#include <HTMLparser.h>

namespace Linked
{
    Q_LOGGING_CATEGORY(LinkedParser, "parser");

    void HTMLparser::processHTML(QNetworkReply *reply)
    {
        auto errorState = reply->error();
        auto url = reply->url();
        qCDebug(LinkedParser) << "   " << url;
        if (errorState != QNetworkReply::NoError)
        {
            qCWarning(LinkedParser) << errorState;
            reply->deleteLater();
            emit sendRequest(url);
            return;
        }

        QString replyStr = reply->readAll();
        reply->deleteLater();
        if (replyStr.contains("description"))
            processDescr(replyStr);
        else
            processTitle(replyStr);
    }

    void HTMLparser::processTitle(QString str)
    {
        str.remove("</li>");
        removeSpaces(str);

        auto data = str.split("<li>");

        qCInfo(LinkedParser) << "data size:" << data.size();

        if (data.size() < 2)
        {
            emit requestFinished();
            return;
        }

        for (auto n = 1; n < data.size(); ++n)
            parseTitle(data[n]);
        emit titlesProcessed();
    }

    void HTMLparser::processDescr(QString str)
    {
        removeSpaces(str);
        parseDescr(str);
    }

    void HTMLparser::parseTitle(QString data)
    {
        QString linkStart = "href=\"", linkEnd = ";";
        QString titleStart = "class=\"base-search-card__title\">\n", titleEnd = "\n";
        QString companyLinkStart = "href=\"", companyLinkEnd = "\"";
        QString companyNameStart = ">\n", companyNameEnd = "\n";
        QString alter_companyNameStart = "class=\"base-search-card__subtitle\">\n", alter_companyNameEnd = "\n";
        QString locationStart = "class=\"job-search-card__location\">\n", locationEnd = "\n";
        QString publishTimeStart = "datetime=\"", publishTimeEnd = "\"";

        Vacancy vacancy;

        vacancy.link = extract(data, linkStart, linkEnd);
        data = data.mid(vacancy.link.size() + data.indexOf(vacancy.link));

        vacancy.title = extract(data, titleStart, titleEnd);
        data = data.mid(vacancy.title.size() + data.indexOf(vacancy.title));

        vacancy.companyLink = extract(data, companyLinkStart, companyLinkEnd);
        data = data.mid(vacancy.companyLink.size() + data.indexOf(vacancy.companyLink));

        if (vacancy.companyLink.contains("http"))
            vacancy.companyName = extract(data, companyNameStart, companyNameEnd);
        else
            vacancy.companyName = extract(data, alter_companyNameStart, alter_companyNameEnd);

        data = data.mid(vacancy.companyName.size() + data.indexOf(vacancy.companyName));

        vacancy.location = extract(data, locationStart, locationEnd);
        data = data.mid(vacancy.location.size() + data.indexOf(vacancy.location));

        QString dateStr = extract(data, publishTimeStart, publishTimeEnd);
        vacancy.date = QDate::fromString(dateStr, "yyyy-MM-dd");

        emit titleReady(vacancy);
    }

    void HTMLparser::parseDescr(QString data)
    {
        QString descriptionStart = "relative overflow-hidden\">\n", descriptionEnd = "\n";
        QString levelStart = "<span class=\"description__job-criteria-text description__job-criteria-text--criteria\">\n", levelEnd = "\n";
        QString employmentTypeStart = "<span class=\"description__job-criteria-text description__job-criteria-text--criteria\">\n", employmentTypeEnd = "\n";

        Vacancy vacancy;

        vacancy.HTML = data;

        vacancy.description = extract(data, descriptionStart, descriptionEnd);
        data = data.mid(vacancy.description.size() + data.indexOf(vacancy.description));

        if (data.contains("Seniority level"))
        {
            vacancy.level = extract(data, levelStart, levelEnd);
            data = data.mid(vacancy.level.size() + data.indexOf(vacancy.level));
        }

        if (data.contains("Employment type"))
        {
            vacancy.employmentType = extract(data, employmentTypeStart, employmentTypeEnd);
            data = data.mid(vacancy.employmentType.size() + data.indexOf(vacancy.employmentType));
        }

        vacancy.description.replace("<br>", "\n");
        vacancy.description.replace("<p>", "\n");
        vacancy.description.replace("<ol>", "\n");
        vacancy.description.replace("<ul>", "\n");
        vacancy.description.replace("<li>", "\n");
        vacancy.description.replace("&nbsp;", " ");

        vacancy.description.remove("</p>");
        vacancy.description.remove("<strong>");
        vacancy.description.remove("</strong>");
        vacancy.description.remove("<span>");
        vacancy.description.remove("</span>");
        vacancy.description.remove("</li>");
        vacancy.description.remove("</ul>");
        vacancy.description.remove("</ol>");
        vacancy.description.remove("<em>");
        vacancy.description.remove("</em>");
        vacancy.description.remove("<u>");
        vacancy.description.remove("</u>");

        removeSpaces(vacancy.description);
        vacancy.description.replace("\n", "\n" + QString(17, ' ') + "| ");

        emit descrReady(vacancy);
    }

    void HTMLparser::removeSpaces(QString &str)
    {
        if (str.size() < 2)
            return;

        qsizetype index = 1;
        while (str.size() > index)
        {
            if ((str[index] == ' ' && str[index - 1] == ' ') ||
                (str[index] == '\n' && str[index - 1] == '\n') ||
                (str[index] == ' ' && str[index - 1] == '\n'))
            {
                str.remove(index, 1);
                continue;
            }
            ++index;
        }
    }

    QString HTMLparser::extract(QString data, const QString &start, const QString &end)
    {
        auto startIndex = data.indexOf(start);
        if (startIndex == -1)
            return QString();

        data = data.mid(startIndex + start.size());
        auto endIndex = data.indexOf(end);
        if (endIndex == -1)
            return QString();

        return data.mid(0, endIndex);
    }

} // namespace Linked