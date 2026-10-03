#include <gtest/gtest.h>

#include <QFile>
#include <QDir>

#include <LinkedInParserStrategy/LinkedInBriefVacancyExtractor.h>


TEST(LinkedInBriefVacancyExtractor, FullBriefs)
{
    QFile file(":/full_briefs.html");
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));

    const QString html = file.readAll();

    const QStringList briefs = extractVacancyBriefs(html);
    ASSERT_EQ(briefs.size(), 10);
}

TEST(LinkedInBriefVacancyExtractor, PartialBriefs)
{
    QFile file(":/partial_briefs.html");
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));

    const QString html = file.readAll();

    const QStringList briefs = extractVacancyBriefs(html);
    ASSERT_EQ(briefs.size(), 6);
}

TEST(LinkedInBriefVacancyExtractor, EmptyBriefs)
{
    QFile file(":/empty_briefs.html");
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));

    const QString html = file.readAll();

    const QStringList briefs = extractVacancyBriefs(html);
    ASSERT_EQ(briefs.size(), 0);
}

TEST(LinkedInBriefVacancyExtractor, NormalUrl1)
{
    QFile file(":/normal_url_1.html");
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));

    LinkedInBriefVacancyExtractor extractor;
    extractor.setHtml(file.readAll());

    const Vacancy vacancy = extractor.vacancy();

    EXPECT_EQ(vacancy.title, "Service Delivery Manager / MSP Administrator");
    EXPECT_EQ(vacancy.link, QUrl("https://rs.linkedin.com/jobs/view/service-delivery-manager-msp-administrator-at-joyful-craftsmen-4464274493"));
    EXPECT_EQ(vacancy.companyName, "Joyful Craftsmen");
    EXPECT_EQ(vacancy.companyLink, QUrl("https://cz.linkedin.com/company/joyful-craftsmen"));
    EXPECT_EQ(vacancy.location, "Serbia");

    EXPECT_TRUE(vacancy.employmentType.isEmpty());
    EXPECT_TRUE(vacancy.skills.isEmpty());
    EXPECT_TRUE(vacancy.description.isEmpty());

    EXPECT_EQ(vacancy.date, QDate::fromString("2026-09-12", Qt::ISODate));

    EXPECT_FALSE(vacancy.industry.has_value());
    EXPECT_FALSE(vacancy.level.has_value());
    EXPECT_FALSE(vacancy.experience.has_value());
    EXPECT_FALSE(vacancy.education.has_value());
    EXPECT_FALSE(vacancy.expirationDate.has_value());
}

TEST(LinkedInBriefVacancyExtractor, NormalUrl2)
{
    QFile file(":/normal_url_2.html");
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));

    LinkedInBriefVacancyExtractor extractor;
    extractor.setHtml(file.readAll());

    const Vacancy vacancy = extractor.vacancy();

    EXPECT_EQ(vacancy.title, "Art Manager/Lead Art Owner (World of Tanks, PC)");
    EXPECT_EQ(vacancy.link, QUrl("https://rs.linkedin.com/jobs/view/art-manager-lead-art-owner-world-of-tanks-pc-at-wargaming-4458880974"));
    EXPECT_EQ(vacancy.companyName, "Wargaming");
    EXPECT_EQ(vacancy.companyLink, QUrl("https://cy.linkedin.com/company/wargaming-net"));
    EXPECT_EQ(vacancy.location, "Belgrade, Serbia");

    EXPECT_TRUE(vacancy.employmentType.isEmpty());
    EXPECT_TRUE(vacancy.skills.isEmpty());
    EXPECT_TRUE(vacancy.description.isEmpty());

    EXPECT_EQ(vacancy.date, QDate::fromString("2026-09-29", Qt::ISODate));

    EXPECT_FALSE(vacancy.industry.has_value());
    EXPECT_FALSE(vacancy.level.has_value());
    EXPECT_FALSE(vacancy.experience.has_value());
    EXPECT_FALSE(vacancy.education.has_value());
    EXPECT_FALSE(vacancy.expirationDate.has_value());
}

TEST(LinkedInBriefVacancyExtractor, AlternateUrl1)
{
    QFile file(":/alternate_url_1.html");
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));

    LinkedInBriefVacancyExtractor extractor;
    extractor.setHtml(file.readAll());

    const Vacancy vacancy = extractor.vacancy();

    EXPECT_EQ(vacancy.title, "Performance Marketing Associate");
    EXPECT_EQ(vacancy.link, QUrl("https://rs.linkedin.com/jobs/view/performance-marketing-associate-4446672722"));

    EXPECT_TRUE(vacancy.companyName.isEmpty());
    EXPECT_TRUE(vacancy.companyLink.isEmpty());

    EXPECT_EQ(vacancy.location, "Belgrade, Serbia");

    EXPECT_TRUE(vacancy.employmentType.isEmpty());
    EXPECT_TRUE(vacancy.skills.isEmpty());
    EXPECT_TRUE(vacancy.description.isEmpty());

    EXPECT_EQ(vacancy.date, QDate::fromString("2026-08-04", Qt::ISODate));
    
    EXPECT_FALSE(vacancy.industry.has_value());
    EXPECT_FALSE(vacancy.level.has_value());
    EXPECT_FALSE(vacancy.experience.has_value());
    EXPECT_FALSE(vacancy.education.has_value());
    EXPECT_FALSE(vacancy.expirationDate.has_value());
}

TEST(LinkedInBriefVacancyExtractor, AlternateUrl2)
{
    QFile file(":/alternate_url_2.html");
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));

    LinkedInBriefVacancyExtractor extractor;
    extractor.setHtml(file.readAll());

    const Vacancy vacancy = extractor.vacancy();

    EXPECT_EQ(vacancy.title, "Performance Creative Strategist — Meta Ads");
    EXPECT_EQ(vacancy.link, QUrl("https://rs.linkedin.com/jobs/view/performance-creative-strategist-—-meta-ads-4472525421"));

    EXPECT_TRUE(vacancy.companyName.isEmpty());
    EXPECT_TRUE(vacancy.companyLink.isEmpty());

    EXPECT_EQ(vacancy.location, "Serbia");

    EXPECT_TRUE(vacancy.employmentType.isEmpty());
    EXPECT_TRUE(vacancy.skills.isEmpty());
    EXPECT_TRUE(vacancy.description.isEmpty());

    EXPECT_EQ(vacancy.date, QDate::fromString("2026-10-01", Qt::ISODate));

    EXPECT_FALSE(vacancy.industry.has_value());
    EXPECT_FALSE(vacancy.level.has_value());
    EXPECT_FALSE(vacancy.experience.has_value());
    EXPECT_FALSE(vacancy.education.has_value());
    EXPECT_FALSE(vacancy.expirationDate.has_value());
}