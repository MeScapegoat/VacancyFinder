#include "Utils.hpp"

#include <QByteArray>
#include <QDir>
#include <QFile>
#include <QLoggingCategory>
#include <QRegularExpression>
#include <QTextDocumentFragment>


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
    return decodeHtml(extractRaw(html, pattern));
}

QString extractRaw(const QString& html, const QString& pattern)
{
    const QRegularExpression regex(pattern, QRegularExpression::DotMatchesEverythingOption);

    const QRegularExpressionMatch match = regex.match(html);

    if (match.hasMatch())
    {
        return match.captured(1).trimmed();
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
    static const QRegularExpression entityPattern(R"(&(?:#[xX][0-9A-Fa-f]+|#\d+|[A-Za-z][A-Za-z0-9]*);?)");

    while (true)
    {
        QString decodedText;
        qsizetype previousEnd = 0;
        QRegularExpressionMatchIterator matches = entityPattern.globalMatch(text);

        while (matches.hasNext())
        {
            const QRegularExpressionMatch match = matches.next();
            const QString entity = match.captured();
            const QString decodedEntity = QTextDocumentFragment::fromHtml(entity).toPlainText();

            decodedText += text.mid(previousEnd, match.capturedStart() - previousEnd);
            decodedText += (decodedEntity.isEmpty() || decodedEntity == entity) ? entity : decodedEntity;
            previousEnd = match.capturedEnd();
        }

        decodedText += text.mid(previousEnd);

        if (decodedText == text)
        {
            return decodedText;
        }

        text.swap(decodedText);
    }
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
