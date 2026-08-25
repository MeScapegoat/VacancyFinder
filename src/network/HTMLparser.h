#pragma once

// std

// Qt
#include <QNetworkReply>
#include <QLoggingCategory>

// custom
#include "../core/Vacancy.h"
#include "../core/SharedContext.h"

class QJsonObject;

namespace Linked
{
    Q_DECLARE_LOGGING_CATEGORY(LinkedParser);

    class SharedContext;

    class HTMLparser : public QObject
    {
        Q_OBJECT

    public:
        HTMLparser(SharedContext *context);
        ~HTMLparser() = default;

    public slots:
        void processHTML(QNetworkReply *reply);

    signals:
        void titleReady(Vacancy);

        void descrStart(QUrl link);
        void descrReady(Vacancy);

        void replyError(QNetworkReply::NetworkError);
        void sendRequest(QUrl);

        void titlesProcessed();
        void requestFinished();

    private:
        SharedContext *context = nullptr;

    private:
        void removeSpaces(QString &);
        QString extract(QString data, const QString &start, const QString &end);
        QString extract(const QString& html, const QString& pattern);
        QStringList extractAll(const QString& html, const QString& pattern);

        static QString decodeHtml(QString text);

        void processTitles(QString html);
        void processDescription(QString html, const Vacancy& vacancy);

        // fills field of Vacancy:
        // title
        // link
        // companyName
        // companyLink
        // location
        // date
        void parseTitle(const QString& html);


        void parseDescr(const QString& html, const Vacancy& vacancy);

        // fills field of Vacancy:
        // industry
        // expirationDate
        // experience
        // education
        // skills
        // employmentType
        // description
        void parseDescrByJson(const QJsonObject& object, Vacancy vacancy);

        // fills field of Vacancy:
        // industry
        // level
        // employmentType
        // description
        void parseDescrByHtml(const QString& html, Vacancy vacancy);
    };
}