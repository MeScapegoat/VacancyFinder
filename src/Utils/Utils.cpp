#include "Utils.hpp"

#include <QByteArray>
#include <QDir>
#include <QFile>
#include <QLoggingCategory>


Q_LOGGING_CATEGORY(UtilsLog, "utils");

QString removeSpaces(QString str)
{
    static const QRegularExpression emptyLinesRegex(R"((?m)^[ \t]*\r?\n)");
    static const QRegularExpression multipleSpacesTabsRegex(R"([ \t]+)");

    str.remove(emptyLinesRegex);
    str.replace(multipleSpacesTabsRegex, " ");

    return str;
}

QString extract(const QString& html, const QString& pattern)
{
    const QRegularExpression regex(pattern, QRegularExpression::DotMatchesEverythingOption);

    const QRegularExpressionMatch match = regex.match(html);

    if (match.hasMatch())
    {
        return decodeHtml(match.captured(1).trimmed());
    }

    return QString();
}

QStringList extractAll(const QString& data, const QString& pattern)
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

QStringList toStringList(const QSet<QUrl>& urlSet)
{
    QStringList result;

    for (const QUrl& url : urlSet)
    {
        result.append(url.toString());
    }

    return result;
}

QSet<QUrl> toUrlSet(const QStringList& stringList)
{
    QSet<QUrl> result;

    for (const QString& string : stringList)
    {
        result.insert(string);
    }

    return result;
}

QString decodeHtml(QString text)
{
    text.replace("&amp;", "&");
    text.replace("&quot;", "\"");
    text.replace("&#39;", "'");
    text.replace("&lt;", "<");
    text.replace("&gt;", ">");
    text.replace("&nbsp;", " ");

    return text;
}

void saveToFile(const QString& str, const QString& fileName, const QString& folderName)
{
    QDir dir;
    dir.mkdir(folderName);
    QFile file;

    file.setFileName(dir.absolutePath() + "/" + folderName + "/" + fileName);

    if (!file.open(QFile::WriteOnly))
    {
        qCWarning(UtilsLog) << "Could not open output file:" << file.fileName() << file.errorString();
        return;
    }

    const QByteArray data = str.toUtf8();
    if (file.write(data) != data.size())
        qCWarning(UtilsLog) << "Could not write output file:" << file.fileName() << file.errorString();

    file.close();
}
