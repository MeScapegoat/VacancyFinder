#pragma once

// std

// Qt
#include <QNetworkReply>
#include <QLoggingCategory>

// custom
#include <Vacancy.h>

namespace Linked
{
    Q_DECLARE_LOGGING_CATEGORY(LinkedParser);

    class HTMLparser : public QObject
    {
        Q_OBJECT

    public:
        HTMLparser();
        ~HTMLparser();

    public slots:
        void processHTML(QNetworkReply *reply);

    signals:
        void titleReady(Vacancy);
        void descrReady(Vacancy);
        void sendRequest(QUrl);

        void titlesProcessed();
        void requestFinished();

    private:
        void removeSpaces(QString &);
        QString extract(QString data, const QString &start, const QString &end);

        void processTitle(QString);
        void processDescr(QString);

        void parseTitle(QString);
        void parseDescr(QString);
    };
}