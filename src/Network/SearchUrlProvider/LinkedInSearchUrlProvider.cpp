#include "LinkedInSearchUrlProvider.h"

#include <QUrlQuery>


namespace
{
    inline constexpr auto BaseUrl = "https://www.linkedin.com/jobs-guest/jobs/api/seeMoreJobPostings/search";
    inline constexpr qsizetype VacanciesPerPage = 10;
}

LinkedInSearchUrlProvider::LinkedInSearchUrlProvider(QObject* parent) :
    AbstractSearchUrlProvider(parent)
{}

QList<QUrl> LinkedInSearchUrlProvider::createSearchUrls(const QString& keywords,
                                                        const QString& location,
                                                        qsizetype maxVacancies) const
{
    QList<QUrl> urls;

    for (qsizetype start = 0; start < maxVacancies; start += VacanciesPerPage)
    {
        QUrl url(BaseUrl);
        QUrlQuery query;
        query.addQueryItem("keywords", QUrl::toPercentEncoding(keywords));
        query.addQueryItem("location", QUrl::toPercentEncoding(location));
        query.addQueryItem("start", QString::number(start));
        url.setQuery(query);

        urls.append(url);
    }

    return urls;
}
