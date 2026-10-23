#include <gtest/gtest.h>

#include <QList>
#include <QString>
#include <QUrl>

#include <LinkedInSearchUrlProvider.h>

TEST(LinkedInSearchUrlProviderTest, ReturnsNoUrlsWhenNoVacanciesAreRequested)
{
    const LinkedInSearchUrlProvider provider;

    EXPECT_TRUE(provider.createSearchUrls(QStringLiteral("C++"), QStringLiteral("Belgrade"), 0).isEmpty());
}

TEST(LinkedInSearchUrlProviderTest, CreatesOneUrlForUpToTenVacancies)
{
    const LinkedInSearchUrlProvider provider;
    const QList<QUrl> urls = provider.createSearchUrls(QStringLiteral("C++"), QStringLiteral("Belgrade"), 10);

    ASSERT_EQ(urls.size(), 1);
    EXPECT_EQ(urls.first().toString(QUrl::FullyEncoded),
              QStringLiteral("https://www.linkedin.com/jobs-guest/jobs/api/seeMoreJobPostings/search?keywords=C%2B%2B&location=Belgrade&start=0"));
}

TEST(LinkedInSearchUrlProviderTest, CreatesAnotherUrlForEachAdditionalPage)
{
    const LinkedInSearchUrlProvider provider;
    const QList<QUrl> urls = provider.createSearchUrls(QStringLiteral("C++"), QStringLiteral("Belgrade"), 21);

    ASSERT_EQ(urls.size(), 3);
    EXPECT_EQ(urls.at(0).query(QUrl::FullyEncoded), QStringLiteral("keywords=C%2B%2B&location=Belgrade&start=0"));
    EXPECT_EQ(urls.at(1).query(QUrl::FullyEncoded), QStringLiteral("keywords=C%2B%2B&location=Belgrade&start=10"));
    EXPECT_EQ(urls.at(2).query(QUrl::FullyEncoded), QStringLiteral("keywords=C%2B%2B&location=Belgrade&start=20"));
}

TEST(LinkedInSearchUrlProviderTest, PercentEncodesKeywordsAndLocation)
{
    const LinkedInSearchUrlProvider provider;
    const QList<QUrl> urls = provider.createSearchUrls(QStringLiteral("C++ Developer"), QStringLiteral("New York"), 1);

    ASSERT_EQ(urls.size(), 1);
    EXPECT_EQ(urls.first().toString(QUrl::FullyEncoded),
              QStringLiteral("https://www.linkedin.com/jobs-guest/jobs/api/seeMoreJobPostings/search?keywords=C%2B%2B%20Developer&location=New%20York&start=0"));
}
