#pragma once

#include <QList>
#include <QUrl>
#include <QObject>

class QString;

class AbstractSearchUrlProvider : public QObject
{
public:
    AbstractSearchUrlProvider(QObject* parent = nullptr) : QObject(parent)
    {};
    virtual ~AbstractSearchUrlProvider() = default;

public:
    virtual QList<QUrl> createSearchUrls(const QString& keywords,
                                         const QString& location,
                                         qsizetype maxVacancies) const = 0;
};
