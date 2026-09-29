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
    m_processedBriefs(),
    m_processedDetails(),
    isInterrupted(false)
{
}

void HTMLparser::reset()
{
    m_processedBriefs.clear();
    m_processedDetails.clear();

    isInterrupted = false;
}

const QSet<QUrl>& HTMLparser::processedBriefs() const
{
    return m_processedBriefs;
}

const QSet<QUrl>& HTMLparser::processedDetails() const
{
    return m_processedDetails;
}

void HTMLparser::processHTML(const QString& html, const QUrl& url)
{
    if (isInterrupted)
    {
        qCDebug(ParserLog) << "Interrupted";
        return;
    }

    emit parsingStarted();

    qCInfo(ParserLog) << url;

    if (html.contains("description"))
    {
        processDetails(html, url);
    }
    else
    {
        const int id = QUrlQuery(url).queryItemValue("start").toInt();

        processBriefs(html, id);
    }

    emit parsingFinished();
}

void HTMLparser::onInterrupt()
{
    isInterrupted = true;

    qCDebug(ParserLog) << "Interrupted";
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
        return;
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

    if (m_processedDetails.contains(url))
    {
        qCDebug(ParserLog) << "Already processed details:" << url;
        return;
    }
    else
    {
        m_processedDetails.insert(url);
    }

    const QString cleanHtml = removeSpaces(html);

    saveToFile(cleanHtml, QString::number(m_processedDetails.size()), DescriptionFolder);

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

    if (m_processedBriefs.contains(vacancy.link)) // TODO: move out of this function
    {
        qCDebug(ParserLog) << "Already processed brief:" << vacancy.link;
        return;
    }
    else
    {
        m_processedBriefs.insert(vacancy.link);
    }
    vacancy.id = m_processedBriefs.size();

    emit briefReady(vacancy);
    emit sendRequest(vacancy.link);
}

void HTMLparser::parseDetails(const QString& html, const QUrl& url)
{
    static constexpr const char* jsonDataPattern = R"REGEX(<script\b[^>]*type="application/ld\+json"[^>]*>(.*?)</script>)REGEX";

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

    vacancy.id = m_processedDetails.size();

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

    Vacancy vacancy;

    vacancy.id = m_processedDetails.size();

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