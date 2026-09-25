#include "HTMLparser.h"

// Qt
#include <QThread>
#include <QRegularExpression>
#include <QJSonDocument>
#include <QJsonObject>
#include <QUrlQuery>

// custom

// this is development code
#include <QDir>
#include <QFile>

const QString TitleFolder = "DEBUG_TITLES";
const QString DescriptionFolder = "DEBUG_DESCRIPTIONS";


Q_LOGGING_CATEGORY(ParserLog, "parser");


HTMLparser::HTMLparser(QObject* parent) :
    QObject(parent),
    maxVacancies(0),
    totalVacancies(0),
    emptyPagesAmount(0),
    processedBriefs(),
    processedDetails(),
    isInterrupted(false)
{
}

void HTMLparser::setMaxVacancies(const unsigned& newMaxVacancies)
{
    maxVacancies = newMaxVacancies;
    totalVacancies = maxVacancies;
}

void HTMLparser::reset()
{
    maxVacancies = 0;
    totalVacancies = 0;
    emptyPagesAmount = 0;

    processedBriefs.clear();
    processedDetails.clear();

    isInterrupted = false;
}

void HTMLparser::processHTML(const QString& html, const QUrl& url)
{
    if (isInterrupted)
    {
        qCDebug(ParserLog) << "Interrupted";
        return;
    }

    qCDebug(ParserLog) << url;

    if (html.contains("description"))
    {
        processDetails(html, url);
    }
    else
    {
        const int id = QUrlQuery(url).queryItemValue("start").toInt();

        processBriefs(html, id);
    }
}

void HTMLparser::onInterrupt()
{
    isInterrupted = true;

    qCDebug(ParserLog) << "Interrupted";
}

void HTMLparser::onRequestsFinished()
{
    qCDebug(ParserLog) << "maxVacancies:        " << maxVacancies;
    qCDebug(ParserLog) << "totalVacancies:      " << totalVacancies;
    qCDebug(ParserLog) << "emptyPagesAmount:    " << emptyPagesAmount;
    qCDebug(ParserLog) << "processedBriefs:     " << processedBriefs.size();
    qCDebug(ParserLog) << "processedDetails:    " << processedDetails.size();

    emit briefsFinished();
    emit allDetailsFinished();
}

QString HTMLparser::removeSpaces(QString str)
{
    static const QRegularExpression emptyLinesRegex(R"((?m)^[ \t]*\r?\n)");
    static const QRegularExpression multipleSpacesTabsRegex(R"([ \t]+)");

    str.remove(emptyLinesRegex);
    str.replace(multipleSpacesTabsRegex, " ");

    return str;
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
    text.replace("&quot;", "\"");
    text.replace("&#39;", "'");
    text.replace("&lt;", "<");
    text.replace("&gt;", ">");
    text.replace("&nbsp;", " ");

    return text;
}

void HTMLparser::processBriefs(const QString& html, const int id)
{
    static const QRegularExpression vacancyRegex(R"(<li\b[^>]*>(.*?)</li>)", QRegularExpression::DotMatchesEverythingOption);

    if (isInterrupted)
    {
        qCDebug(ParserLog) << "Interrupted";
        return;
    }

    const QString cleanHtml = removeSpaces(html);

    saveToFile(cleanHtml, QString::number(id), TitleFolder);

    QRegularExpressionMatchIterator it = vacancyRegex.globalMatch(cleanHtml);
    QStringList data;

    while (it.hasNext())
    {
        const QRegularExpressionMatch match = it.next();

        data.emplaceBack(match.captured(1));
    }

    if (data.isEmpty())
    {
        qCDebug(ParserLog) << "EMPTY:" << id;
        ++emptyPagesAmount;
        return;
    }

    if (data.size() < 10)
    {
        qCDebug(ParserLog) << "LESS THAN 10:" << data.size() << id;
        totalVacancies = id + data.size();
    }

    for (const QString& title : data)
    {
        parseBrief(title);
    }
}

void HTMLparser::processDetails(const QString& html, const QUrl& url)
{
    if (isInterrupted)
    {
        qCDebug(ParserLog) << "Interrupted";
        return;
    }

    if (processedDetails.size() >= totalVacancies)
    {
        qCInfo(ParserLog) << "All Details Finished!";
        emit allDetailsFinished();
    }

    if (processedDetails.contains(url))
    {
        qCDebug(ParserLog) << "ALREADY PROCESSED DESCRIPTION:" << url;
        return;
    }
    else
    {
        processedDetails.insert(url);
    }

    const QString cleanHtml = removeSpaces(html);

    saveToFile(cleanHtml, QString::number(processedDetails.size()), DescriptionFolder);

    parseDetails(cleanHtml, url);
}

void HTMLparser::parseBrief(const QString& html)
{
    static constexpr const char* titlePattern = R"(<h3\b[^>]*class="base-search-card__title"[^>]*>(.*?)</h3>)";
    static constexpr const char* linkPattern = R"REGEX(<a\b[^>]*class="base-card__full-link[^"]*"[^>]*href="([^"]+)")REGEX";
    static constexpr const char* alternateLinkPattern = R"REGEX(<a\s+href="(https://rs\.linkedin\.com/jobs/view/[^"]+)")REGEX";

    static constexpr const char* companyNamePattern = R"(<h4\b[^>]*class="base-search-card__subtitle"[^>]*>.*?<a\b[^>]*>(.*?)</a>)";
    static constexpr const char* companyLinkPattern = R"REGEX(<h4\b[^>]*class="base-search-card__subtitle"[^>]*>.*?<a\b[^>]*href="([^"]+)")REGEX";

    static constexpr const char* locationPattern = R"(<span\b[^>]*class="job-search-card__location"[^>]*>(.*?)</span>)";
    static constexpr const char* datePattern = R"REGEX(<time\b[^>]*datetime="([^"]+)")REGEX";


    static constexpr const char* dateFormat = "dd-MM-yyyy";

    if (processedBriefs.size() >= totalVacancies)
    {
        qCDebug(ParserLog) << "Briefs Finished!";
        emit briefsFinished();
        return;
    }

    Vacancy vacancy;
    vacancy.title = extract(html, titlePattern);

    vacancy.link = extract(html, linkPattern);

    if (!vacancy.link.isValid())
    {
        vacancy.link = extract(html, alternateLinkPattern);
    }
    vacancy.link.setQuery(QString());

    vacancy.companyName = extract(html, companyNamePattern);

    QUrl companyLink = extract(html, companyLinkPattern);
    companyLink.setQuery(QString());

    vacancy.companyLink = companyLink;

    vacancy.location = extract(html, locationPattern);

    const QString date = extract(html, datePattern);
    vacancy.date = QDate::fromString(date, Qt::ISODate);

    if (processedBriefs.contains(vacancy.link))
    {
        qCDebug(ParserLog) << "ALREADY PROCESSED THIS BRIEF:" << vacancy.link;
        return;
    }
    else
    {
        processedBriefs.insert(vacancy.link);
    }
    vacancy.id = processedBriefs.size();

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
            qCDebug(ParserLog) << list;
            qCDebug(ParserLog) << "TEXT";
        }
    }

    //qCDebug(ParserLog) << vacancy;

    emit briefReady(vacancy);
    emit sendRequest(vacancy.link);

    if (processedBriefs.size() >= totalVacancies)
    {
        qCInfo(ParserLog) << "Briefs Finished!";
        emit briefsFinished();
    }
}

void HTMLparser::parseDetails(const QString& html, const QUrl& url)
{
    static constexpr const char* jsonDataPattern = R"REGEX(<script\b[^>]*type="application/ld\+json"[^>]*>(.*?)</script>)REGEX";

    const QStringList list = extractAll(html, jsonDataPattern);
    if (list.size() > 1)
    {
        qCDebug(ParserLog) << "MORE THAN ONE:";
        qCDebug(ParserLog) << list;
        qCDebug(ParserLog) << "TEXT";
    }

    const QString jsonText = extract(html, jsonDataPattern);
    const QJsonDocument document = QJsonDocument::fromJson(jsonText.toUtf8());

    Vacancy vacancy;

    if (!document.isNull() && document.isObject())
    {
        vacancy = parseDetailsByJson(document.object(), url);
    }
    else
    {
        vacancy = parseDetailsByHtml(html, url);
    }

    emit detailsReady(vacancy);

    if (processedDetails.size() >= totalVacancies)
    {
        qCInfo(ParserLog) << "All Details Finished!";
        emit allDetailsFinished();
    }
}

Vacancy HTMLparser::parseDetailsByJson(const QJsonObject& object, const QUrl& url)
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


    Vacancy vacancy;

    vacancy.id = processedDetails.size();

    vacancy.title = object[titleFieldName].toString();
    vacancy.link = url;

    vacancy.companyName = object[organizationFieldName][organizationNameFieldName].toString();
    vacancy.companyLink = object[organizationFieldName][organizationLinkFieldName].toString();

    vacancy.location = object[locationFieldName][locationAddressFieldName][locationAddressLocalityFieldName].toString();

    vacancy.employmentType = object[employmentTypeFieldName].toString();
    vacancy.skills = object[skillsFieldName].toString();

    vacancy.description = object[descriptionTypeFieldName].toString();

    vacancy.date = QDate::fromString(object[dateFieldName].toString(), Qt::ISODate);

    vacancy.industry = object[industryFieldName].toString();
    //vacancy.level;
    vacancy.experience = object[experienceFieldName][experienceMonthsFieldName].toInt();
    vacancy.education = object[educationFieldName][educationCategoryFieldName].toString();
    vacancy.expirationDate = QDate::fromString(object[expirationDateFieldName].toString(), Qt::ISODate);;
    
    static constexpr const char* typeFieldName = "@type";

    //qCDebug(ParserLog).noquote() << QJsonDocument(object).toJson(QJsonDocument::Indented);

    if (!object[educationFieldName][typeFieldName].toString().isEmpty() && object[educationFieldName][typeFieldName].toString() != "EducationalOccupationalCredential")
    {
        qCDebug(ParserLog) << "NOT EducationalOccupationalCredential";
    }

    if (!object[experienceFieldName][typeFieldName].toString().isEmpty() && object[experienceFieldName][typeFieldName].toString() != "OccupationalExperienceRequirements")
    {
        qCDebug(ParserLog) << "NOT OccupationalExperienceRequirements";
    }

    if (!object[locationFieldName][typeFieldName].toString().isEmpty() && object[locationFieldName][typeFieldName].toString() != "Place")
    {
        qCDebug(ParserLog) << "NOT Place";
    }

    if (!object[organizationFieldName][typeFieldName].toString().isEmpty() && object[organizationFieldName][typeFieldName].toString() != "Organization")
    {
        qCDebug(ParserLog) << "NOT Organization";
    }

    return vacancy;
}

Vacancy HTMLparser::parseDetailsByHtml(const QString& html, const QUrl& url)
{
    static constexpr const char* titlePattern = R"REGEX(<h1\b[^>]*class="top-card-layout__title[^"]*"[^>]*>(.*?)</h1>)REGEX";
    static constexpr const char* linkPattern = R"REGEX(<link\b[^>]*rel="canonical"[^>]*href="([^"]+)")REGEX";

    static constexpr const char* companyNamePattern = R"REGEX(<a\b[^>]*class="topcard__org-name-link[^"]*"[^>]*>(.*?)</a>)REGEX";
    static constexpr const char* companyLinkPattern = R"REGEX(<a\b[^>]*class="topcard__org-name-link[^"]*"[^>]*href="([^"]+)")REGEX";

    static constexpr const char* locationPattern = R"REGEX(<span\b[^>]*class="topcard__flavor[^"]*topcard__flavor--bullet[^"]*"[^>]*>(.*?)</span>)REGEX";

    static constexpr const char* descriptionPattern = R"REGEX(<div\b[^>]*class="show-more-less-html__markup[^"]*"[^>]*>(.*?)</div>)REGEX";

    static constexpr const char* jobCriteriaPattern = R"REGEX(<span\b[^>]*class="description__job-criteria-text[^"]*description__job-criteria-text--criteria[^"]*"[^>]*>(.*?)</span>)REGEX";

    //static constexpr const char* isClosedJobStr = "closed-job";

    const QStringList jobCriteriaList = extractAll(html, jobCriteriaPattern);

    if (jobCriteriaList.size() > 4)
    {
        qCDebug(ParserLog) << "MORE THAN FOUR:";
        qCDebug(ParserLog) << jobCriteriaList;
        qCDebug(ParserLog) << "TEXT";
    }
    else if (jobCriteriaList.size() < 4)
    {
        qCDebug(ParserLog) << "LESS THAN FOUR:";
        qCDebug(ParserLog) << jobCriteriaList;
        qCDebug(ParserLog) << "TEXT";
    }

    Vacancy vacancy;

    vacancy.id = processedDetails.size();

    vacancy.title = extract(html, titlePattern);
    vacancy.link = url;

    vacancy.companyName = extract(html, companyNamePattern);
    vacancy.companyLink = extract(html, companyLinkPattern);
    vacancy.companyLink.setQuery(QString());

    vacancy.location = extract(html, locationPattern);

    if (jobCriteriaList.size() > 1) vacancy.employmentType = jobCriteriaList[1];
    //skills;
    vacancy.description = extract(html, descriptionPattern);
    //date;
    if (jobCriteriaList.size() > 3) vacancy.industry = jobCriteriaList[3];
    if (jobCriteriaList.size() > 0) vacancy.level = jobCriteriaList[0];
    //experience;
    //education;
    //expirationDate;

    QList<QStringList> lists;
    lists.append(extractAll(html, descriptionPattern));

    for (const QStringList list : lists)
    {
        if (list.size() > 1)
        {
            qCDebug(ParserLog) << "MORE THAN ONE:";
            qCDebug(ParserLog) << list;
            qCDebug(ParserLog) << "TEXT";
        }
    }

    return vacancy;
}




void saveToFile(const QString& html, const QString& name, const QString& folder)
{
    QDir dir;
    dir.mkdir(folder);
    QFile file;

    file.setFileName(dir.absolutePath() + "/" + folder + "/" + name + ".html");

    if (file.open(QFile::WriteOnly))
        file.write(html.toUtf8());
    else
    {
        qInfo() << "couldnt open file!" << file.fileName();
        return;
    }
    file.close();
}