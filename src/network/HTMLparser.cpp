// std

// Qt
#include <QThread>
#include <QRegularExpression>
#include <QJSonDocument>
#include <QJsonObject>

// custom
#include "HTMLparser.h"

// this is development code
#include <QDir>
#include <QFile>
quint64 titleAmount = 0;
quint64 descrAmount = 0;
const QString TitleFolder = "DEBUG_TITLES";
const QString DescriptionFolder = "DEBUG_DESCRIPTIONS";
void saveToFile(QString str, QString folder)
{
    QDir dir;
    dir.mkdir(folder);
    QFile file;
    if (folder == TitleFolder)
        file.setFileName(dir.absolutePath() + "/" + folder + "/" + QString::number(titleAmount++) + ".html");
    else if (folder == DescriptionFolder)
        file.setFileName(dir.absolutePath() + "/" + folder + "/" + QString::number(descrAmount++) + ".html");
    else
    {
        qInfo() << "Unexpected folder!" << folder;
        return;
    }

    if (file.open(QFile::WriteOnly))
        file.write(str.toUtf8());
    else
    {
        qInfo() << "couldnt open file!" << file.fileName();
        return;
    }
    file.close();
}

namespace Linked
{
    Q_LOGGING_CATEGORY(LinkedParser, "parser");

    HTMLparser::HTMLparser(SharedContext *SharedContext) : 
        context(SharedContext) 
    {
    }

    void HTMLparser::processHTML(QNetworkReply *reply)
    {
        const QNetworkReply::NetworkError errorState = reply->error();

        const QUrl url = reply->url();
        qCDebug(LinkedParser) << url;

        emit replyError(errorState);
        if (errorState != QNetworkReply::NoError)
        {
            qCWarning(LinkedParser) << errorState;

            //qDebug() << "URL:" << reply->url();
            qCDebug(LinkedParser) << "Error:" << reply->error();
            qCDebug(LinkedParser) << "Error string:" << reply->errorString();
            qCDebug(LinkedParser) << "HTTP status:"<< reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);

            qCDebug(LinkedParser) << "Content type:"<< reply->header(QNetworkRequest::ContentTypeHeader);

            qCDebug(LinkedParser) << "Content length:"<< reply->header(QNetworkRequest::ContentLengthHeader);

            qCDebug(LinkedParser) << "Raw headers:";

            for (const auto& header : reply->rawHeaderPairs())
                qCDebug(LinkedParser) << header.first << ":" << header.second;

            const QByteArray data = reply->readAll();

            qCDebug(LinkedParser) << "Received bytes:" << data.size();
            qCDebug(LinkedParser) << "First 500 bytes:" << data.left(500);

            reply->deleteLater();

            emit sendRequest(url);
            return;
        }

        const QString replyStr = reply->readAll();
        reply->deleteLater();

        

        if (replyStr.contains("description"))
        {
            Vacancy vacancy;
            vacancy.link = url;
            //vacancy.link.setQuery(QString());
            processDescription(replyStr, vacancy);
        }
        else
        {
            processTitles(replyStr);
        }
    }

    void HTMLparser::processTitles(QString html)
    {
        static const QRegularExpression vacancyRegex(R"(<li\b[^>]*>(.*?)</li>)", QRegularExpression::DotMatchesEverythingOption);


        removeSpaces(html);

        saveToFile(html, TitleFolder);


        QRegularExpressionMatchIterator it = vacancyRegex.globalMatch(html);
        QStringList data;

        while (it.hasNext())
        {
            const QRegularExpressionMatch match = it.next();

            data.emplaceBack(match.captured(1));
        }

        qCInfo(LinkedParser) << "Titles to be processed:" << data.size();

        if (data.isEmpty())
        {
            emit requestFinished();
            return;
        }

        for (const QString& title : data)
        {
            parseTitle(title);
        }   

        emit titlesProcessed();
    }

    void HTMLparser::processDescription(QString html, const Vacancy& vacancy)
    {
        emit descrStart(vacancy.link);

        removeSpaces(html);

        saveToFile(html, DescriptionFolder);

        parseDescr(html, vacancy);
    }

    void HTMLparser::parseTitle(const QString& html)
    {
        static constexpr const char* titlePattern = R"(<h3\b[^>]*class="base-search-card__title"[^>]*>(.*?)</h3>)";
        static constexpr const char* linkPattern = R"REGEX(<a\b[^>]*class="base-card__full-link[^"]*"[^>]*href="([^"]+)")REGEX";

        static constexpr const char* companyNamePattern = R"(<h4\b[^>]*class="base-search-card__subtitle"[^>]*>.*?<a\b[^>]*>(.*?)</a>)";
        static constexpr const char* companyLinkPattern = R"REGEX(<h4\b[^>]*class="base-search-card__subtitle"[^>]*>.*?<a\b[^>]*href="([^"]+)")REGEX";
        
        static constexpr const char* locationPattern = R"(<span\b[^>]*class="job-search-card__location"[^>]*>(.*?)</span>)";
        static constexpr const char* datePattern = R"REGEX(<time\b[^>]*datetime="([^"]+)")REGEX";


        static constexpr const char* dateFormat = "dd-MM-yyyy";

        Vacancy vacancy;
        vacancy.title = extract(html, titlePattern);

        vacancy.link = extract(html, linkPattern);
        vacancy.link.setQuery(QString());

        vacancy.companyName = extract(html, companyNamePattern);
        
        vacancy.companyLink = QUrl(extract(html, companyLinkPattern));
        vacancy.companyLink.setQuery(QString());

        vacancy.location = extract(html, locationPattern);

        const QString date = extract(html, datePattern);
        vacancy.date = QDate::fromString(date, Qt::ISODate);

        QList<QStringList> lists;
        lists.append(extractAll(html, linkPattern));
        lists.append(extractAll(html, titlePattern));
        lists.append(extractAll(html, companyLinkPattern));
        lists.append(extractAll(html, companyNamePattern));
        lists.append(extractAll(html, locationPattern));
        lists.append(extractAll(html, datePattern));

        for (const QStringList list : lists)
        {
            if (list.size() > 1)
            {
                qCDebug(LinkedParser) << list;
                qCDebug(LinkedParser) << "GAY";
            }
        }

        //qCDebug(LinkedParser) << vacancy;

        emit titleReady(vacancy);
        emit sendRequest(vacancy.link);
    }

    void HTMLparser::parseDescr(const QString& html, const Vacancy& vacancy)
    {
        static constexpr const char* jsonDataPattern = R"REGEX(<script\b[^>]*type="application/ld\+json"[^>]*>(.*?)</script>)REGEX";

        const QStringList list = extractAll(html, jsonDataPattern);
        if (list.size() > 1)
        {
            qCDebug(LinkedParser) << "MORE THAN ONE:";
            qCDebug(LinkedParser) << list;
            qCDebug(LinkedParser) << "GAY";
        }

        const QString jsonText = extract(html, jsonDataPattern);
        const QJsonDocument document = QJsonDocument::fromJson(jsonText.toUtf8());
        
        if (!document.isNull() && document.isObject())
        {
            parseDescrByJson(document.object(), vacancy);
        }
        else
        {
            parseDescrByHtml(html, vacancy);
        }
    }

    void HTMLparser::parseDescrByJson(const QJsonObject& object, Vacancy vacancy)
    {
        static constexpr const char* titleFieldName = "title";

        static constexpr const char* organizationFieldName = "hiringOrganization";
        static constexpr const char* organizationNameFieldName = "name";
        static constexpr const char* organizationLinkFieldName = "sameAs";

        static constexpr const char* industryFieldName = "industry";

        static constexpr const char* locationFieldName = "jobLocation";
        static constexpr const char* locationAddressFieldName = "address";
        static constexpr const char* locationAddressLocalityFieldName = "addressLocality";

        static constexpr const char* dateFieldName = "datePosted";
        static constexpr const char* expirationDateFieldName = "validThrough";

        static constexpr const char* experienceFieldName = "experienceRequirements";
        static constexpr const char* experienceMonthsFieldName = "monthsOfExperience";

        static constexpr const char* educationFieldName = "educationRequirements"; 
        static constexpr const char* educationCategoryFieldName = "credentialCategory";

        static constexpr const char* skillsFieldName = "skills";

        static constexpr const char* employmentTypeFieldName = "employmentType";

        static constexpr const char* descriptionTypeFieldName = "description";        

        //vacancy.title;
        //vacancy.link;
        //vacancy.companyName;
        //vacancy.companyLink;
        //vacancy.location;
        //vacancy.date;

        //vacancy.title = object[titleFieldName].toString();
        //vacancy.link; // ??
        //vacancy.companyName = object[organizationFieldName][organizationNameFieldName].toString();
        vacancy.industry = object[industryFieldName].toString();
        //vacancy.companyLink = object[organizationFieldName][organizationLinkFieldName].toString();
        //vacancy.location = object[locationFieldName][locationAddressFieldName][locationAddressLocalityFieldName].toString();
        //vacancy.date = QDate::fromString(object[dateFieldName].toString(), Qt::ISODate);
        vacancy.expirationDate = QDate::fromString(object[expirationDateFieldName].toString(), Qt::ISODate);;
        vacancy.experience = object[experienceFieldName][experienceMonthsFieldName].toInt();
        vacancy.level; // ??
        vacancy.education = object[educationFieldName][educationCategoryFieldName].toString();
        vacancy.skills = object[skillsFieldName].toString();
        vacancy.employmentType = object[employmentTypeFieldName].toString();
        vacancy.description = object[descriptionTypeFieldName].toString();

        static constexpr const char* typeFieldName = "@type";

        qCDebug(LinkedParser).noquote() << QJsonDocument(object).toJson(QJsonDocument::Indented);

        if (!object[skillsFieldName].toString().isEmpty())
        {
            qCDebug(LinkedParser).noquote() << "NOT EMPTY SKILLS";
        }

        if (!object[educationFieldName][typeFieldName].toString().isEmpty() && object[educationFieldName][typeFieldName].toString() != "EducationalOccupationalCredential")
        {
            qCDebug(LinkedParser) << "NOT EducationalOccupationalCredential";
        }

        if (!object[experienceFieldName][typeFieldName].toString().isEmpty() && object[experienceFieldName][typeFieldName].toString() != "OccupationalExperienceRequirements")
        {
            qCDebug(LinkedParser) << "NOT OccupationalExperienceRequirements";
        }

        if (!object[locationFieldName][typeFieldName].toString().isEmpty() && object[locationFieldName][typeFieldName].toString() != "Place")
        {
            qCDebug(LinkedParser) << "NOT Place";
        }

        if (!object[organizationFieldName][typeFieldName].toString().isEmpty() && object[organizationFieldName][typeFieldName].toString() != "Organization")
        {
            qCDebug(LinkedParser) << "NOT Organization";
        }

        emit descrReady(vacancy);
    }

    void HTMLparser::parseDescrByHtml(const QString & html, Vacancy vacancy)
    {
        static constexpr const char* titlePattern = R"REGEX(<h1\b[^>]*class="top-card-layout__title[^"]*"[^>]*>(.*?)</h1>)REGEX";
        static constexpr const char* linkPattern = R"REGEX(<link\b[^>]*rel="canonical"[^>]*href="([^"]+)")REGEX";

        static constexpr const char* companyNamePattern = R"REGEX(<a\b[^>]*class="topcard__org-name-link[^"]*"[^>]*>(.*?)</a>)REGEX";
        static constexpr const char* companyLinkPattern = R"REGEX(<a\b[^>]*class="topcard__org-name-link[^"]*"[^>]*href="([^"]+)")REGEX";

        static constexpr const char* locationPattern = R"REGEX(<span\b[^>]*class="topcard__flavor[^"]*topcard__flavor--bullet[^"]*"[^>]*>(.*?)</span>)REGEX";

        static constexpr const char* descriptionPattern = R"REGEX(<div\b[^>]*class="show-more-less-html__markup[^"]*"[^>]*>(.*?)</div>)REGEX";

        static constexpr const char* jobCriteriaPattern = R"REGEX(<span\b[^>]*class="description__job-criteria-text[^"]*description__job-criteria-text--criteria[^"]*"[^>]*>(.*?)</span>)REGEX";

        const QStringList jobCriteriaList = extractAll(html, jobCriteriaPattern);

        // UNFILLED:
        vacancy.expirationDate;
        vacancy.experience;
        vacancy.education;
        vacancy.skills;

        //vacancy.title = extract(html, titlePattern);
        //vacancy.link = extract(html, linkPattern);
        //vacancy.link.setQuery(QString());

        //vacancy.companyName = extract(html, companyNamePattern);
        vacancy.industry = jobCriteriaList[3];
        //vacancy.companyLink = extract(html, companyLinkPattern);
        //vacancy.companyLink.setQuery(QString());

        //vacancy.location = extract(html, locationPattern);
        vacancy.level = jobCriteriaList[0];
        vacancy.employmentType = jobCriteriaList[1];
        vacancy.description = extract(html, descriptionPattern);

        QList<QStringList> lists;
        //lists.append(extractAll(html, titlePattern));
        //lists.append(extractAll(html, linkPattern));
        //lists.append(extractAll(html, companyNamePattern));
        //lists.append(extractAll(html, companyLinkPattern));
        //lists.append(extractAll(html, locationPattern));
        //lists.append(extractAll(newData, datePattern));
        //lists.append(extractAll(newData, levelPattern));
        //lists.append(extractAll(newData, employmentTypePattern));
        lists.append(extractAll(html, descriptionPattern));

        for (const QStringList list : lists)
        {
            if (list.size() > 1)
            {
                qCDebug(LinkedParser) << "MORE THAN ONE:";
                qCDebug(LinkedParser) << list;
                qCDebug(LinkedParser) << "GAY";
            }
        }

        emit descrReady(vacancy);
    }

    void HTMLparser::removeSpaces(QString &str)
    {
        static const QRegularExpression emptyLinesRegex(R"((?m)^[ \t]*\r?\n)");
        static const QRegularExpression multipleSpacesTabsRegex(R"([ \t]+)");

        str.remove(emptyLinesRegex);
        str.replace(multipleSpacesTabsRegex, " ");
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

    QString HTMLparser::extract(const QString& html, const QString& pattern)
    {
        const QRegularExpression regex(pattern, QRegularExpression::DotMatchesEverythingOption);

        const QRegularExpressionMatch match = regex.match(html);

        if (match.hasMatch())
        {
            return decodeHtml(match.captured(1).trimmed());
        }

        return QString();
    }

    QStringList HTMLparser::extractAll(const QString& data, const QString& pattern)
    {
        const QRegularExpression regex(pattern, QRegularExpression::DotMatchesEverythingOption);

        QStringList result;

        QRegularExpressionMatchIterator it = regex.globalMatch(data);

        while (it.hasNext())
        {
            const QRegularExpressionMatch match = it.next();
            result.append(decodeHtml(match.captured(1).trimmed()));
        }

        return result;
    }

    QString HTMLparser::decodeHtml(QString text)
    {
        text.replace("&amp;", "&");
        //text.replace("&quot;", "\"");
        //text.replace("&#39;", "'");
        //text.replace("&lt;", "<");
        //text.replace("&gt;", ">");
        //text.replace("&nbsp;", " ");

        return text;
    }

} // namespace Linked