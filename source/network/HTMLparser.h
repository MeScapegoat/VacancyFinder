#pragma once

// std

// Qt
#include <QNetworkReply>
#include <QLoggingCategory>

// custom
#include "../core/Vacancy.h"
#include "../core/SharedContext.h"

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

    public:
        qsizetype descriptionID = 0;

    public slots:
        void processHTML(QNetworkReply *reply);

    signals:
        void titleReady(Vacancy);

        void descrStart(qsizetype);
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

        void processTitle(QString);
        void processDescr(QString);

        void parseTitle(QString);
        void parseDescr(QString);
    };
}