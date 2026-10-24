#include <gtest/gtest.h>

#include <QDate>
#include <QString>
#include <QUrl>

#include <QtTestPrinters.h>

#include <Vacancy.h>

TEST(VacancyTest, CountSearchesTitleAndDescriptionCaseInsensitively)
{
    Vacancy vacancy;
    vacancy.title = QStringLiteral("C++ Engineer");
    vacancy.description = QStringLiteral("C++ development with Qt");
    vacancy.companyName = QStringLiteral("C++ Tools");

    EXPECT_EQ(vacancy.count(QStringLiteral("C++")), 2);
    EXPECT_EQ(vacancy.count(QStringLiteral("c++"), Qt::CaseSensitive), 0);
}

TEST(VacancyTest, ContainsHonorsCaseSensitivity)
{
    Vacancy vacancy;
    vacancy.title = QStringLiteral("Senior Engineer");
    vacancy.description = QStringLiteral("Build Qt software");

    EXPECT_TRUE(vacancy.contains(QStringLiteral("Engineer")));
    EXPECT_FALSE(vacancy.contains(QStringLiteral("engineer")));
    EXPECT_TRUE(vacancy.contains(QStringLiteral("engineer"), Qt::CaseInsensitive));
    EXPECT_FALSE(vacancy.contains(QStringLiteral("Java")));
}

TEST(VacancyTest, FillEmptyFieldsFromFallbackWithoutOverwritingExistingValues)
{
    Vacancy vacancy;
    vacancy.id = 100;
    vacancy.title = QStringLiteral("Original title");
    vacancy.companyName = QStringLiteral("Original company");
    vacancy.description = QStringLiteral("Original description");
    vacancy.date = QDate(2024, 1, 2);
    vacancy.industry = QStringLiteral("Original industry");
    vacancy.experience = Months{36};

    Vacancy fallback;
    fallback.id = 200;
    fallback.title = QStringLiteral("Fallback title");
    fallback.link = QUrl(QStringLiteral("https://example.com/jobs/1"));
    fallback.companyName = QStringLiteral("Fallback company");
    fallback.companyLink = QUrl(QStringLiteral("https://example.com/company"));
    fallback.location = QStringLiteral("Budapest");
    fallback.employmentType = QStringLiteral("Full-time");
    fallback.skills = QStringLiteral("C++; Qt");
    fallback.description = QStringLiteral("Fallback description");
    fallback.date = QDate(2025, 3, 4);
    fallback.industry = QStringLiteral("Fallback industry");
    fallback.level = QStringLiteral("Senior");
    fallback.experience = Months{60};
    fallback.education = QStringLiteral("Bachelor degree");
    fallback.expirationDate = QDate(2025, 6, 1);

    vacancy.fillEmptyFrom(fallback);

    EXPECT_EQ(vacancy.id, 100);
    EXPECT_EQ(vacancy.title, QStringLiteral("Original title"));
    EXPECT_EQ(vacancy.link, fallback.link);
    EXPECT_EQ(vacancy.companyName, QStringLiteral("Original company"));
    EXPECT_EQ(vacancy.companyLink, fallback.companyLink);
    EXPECT_EQ(vacancy.location, QStringLiteral("Budapest"));
    EXPECT_EQ(vacancy.employmentType, QStringLiteral("Full-time"));
    EXPECT_EQ(vacancy.skills, QStringLiteral("C++; Qt"));
    EXPECT_EQ(vacancy.description, QStringLiteral("Original description"));
    EXPECT_EQ(vacancy.date, QDate(2024, 1, 2));
    ASSERT_TRUE(vacancy.industry.has_value());
    EXPECT_EQ(vacancy.industry.value(), QStringLiteral("Original industry"));
    ASSERT_TRUE(vacancy.level.has_value());
    EXPECT_EQ(vacancy.level.value(), QStringLiteral("Senior"));
    ASSERT_TRUE(vacancy.experience.has_value());
    EXPECT_EQ(vacancy.experience.value(), Months{36});
    ASSERT_TRUE(vacancy.education.has_value());
    EXPECT_EQ(vacancy.education.value(), QStringLiteral("Bachelor degree"));
    ASSERT_TRUE(vacancy.expirationDate.has_value());
    EXPECT_EQ(vacancy.expirationDate.value(), QDate(2025, 6, 1));
}
