#pragma once

// std
#include <QNetworkReply>

// Qt

// custom
#include <Vacancy.h>

namespace Linked
{
    class HTMLparser : public QObject
    {
        Q_OBJECT

    public:
        HTMLparser() = default;
        ~HTMLparser() = default;

    public slots:
        void processHTML(QNetworkReply *reply);

    signals:
        void titleReady(Vacancy);
        void descrReady(Vacancy);

    private:
        void removeSpaces(QString &);
        QString extract(QString data, const QString &start, const QString &end);

        void processTitle(QString);
        void processDescr(QString);

        void parseTitle(QString);
        void parseDescr(QString);
    };
}