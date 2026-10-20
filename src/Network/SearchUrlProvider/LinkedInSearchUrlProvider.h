#pragma once

#include "AbstractSearchUrlProvider.h"

#include <QList>
#include <QString>
#include <QUrl>


class LinkedInSearchUrlProvider : public AbstractSearchUrlProvider
{
public:
    LinkedInSearchUrlProvider(QObject* parent = nullptr);

public:
    QList<QUrl> createSearchUrls(const QString& keywords,
                                 const QString& location,
                                 qsizetype maxVacancies) const override;
};
