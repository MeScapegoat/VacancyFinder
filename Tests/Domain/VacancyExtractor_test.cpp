#include <gtest/gtest.h>

#include <optional>

#include <QDate>
#include <QString>
#include <QUrl>

#include <QtTestPrinters.h>

#include <VacancyExtractor.h>

namespace
{
    class VacancyExtractorStub final : public VacancyExtractor
    {
    protected:
        QString title() const override { return QStringLiteral("Software Engineer"); }
        QUrl url() const override { return QUrl(QStringLiteral("https://example.com/jobs/1")); }
        QString companyName() const override { return QStringLiteral("Acme"); }
        QUrl companyUrl() const override { return QUrl(QStringLiteral("https://example.com/company/acme")); }
        QString location() const override { return QStringLiteral("Budapest"); }
        QString employmentType() const override { return QStringLiteral("Full-time"); }
        QString skills() const override { return QStringLiteral("C++; Qt"); }
        QString description() const override { return QStringLiteral("Build reliable software."); }
        QDate date() const override { return QDate(2025, 1, 2); }
        std::optional<QString> industry() const override { return QStringLiteral("Software Development"); }
        std::optional<QString> level() const override { return QStringLiteral("Senior"); }
        std::optional<Months> experience() const override { return Months{60}; }
        std::optional<QString> education() const override { return QStringLiteral("Bachelor degree"); }
        std::optional<QDate> expirationDate() const override { return QDate(2025, 6, 1); }
    };
}

TEST(VacancyExtractorTest, CollectsSubclassValuesIntoVacancy)
{
    const VacancyExtractorStub extractor;

    const Vacancy vacancy = extractor.vacancy();

    EXPECT_EQ(vacancy.title, QStringLiteral("Software Engineer"));
    EXPECT_EQ(vacancy.url, QUrl(QStringLiteral("https://example.com/jobs/1")));
    EXPECT_EQ(vacancy.companyName, QStringLiteral("Acme"));
    EXPECT_EQ(vacancy.companyUrl, QUrl(QStringLiteral("https://example.com/company/acme")));
    EXPECT_EQ(vacancy.location, QStringLiteral("Budapest"));
    EXPECT_EQ(vacancy.employmentType, QStringLiteral("Full-time"));
    EXPECT_EQ(vacancy.skills, QStringLiteral("C++; Qt"));
    EXPECT_EQ(vacancy.description, QStringLiteral("Build reliable software."));
    EXPECT_EQ(vacancy.date, QDate(2025, 1, 2));
    ASSERT_TRUE(vacancy.industry.has_value());
    EXPECT_EQ(vacancy.industry.value(), QStringLiteral("Software Development"));
    ASSERT_TRUE(vacancy.level.has_value());
    EXPECT_EQ(vacancy.level.value(), QStringLiteral("Senior"));
    ASSERT_TRUE(vacancy.experience.has_value());
    EXPECT_EQ(vacancy.experience.value(), Months{60});
    ASSERT_TRUE(vacancy.education.has_value());
    EXPECT_EQ(vacancy.education.value(), QStringLiteral("Bachelor degree"));
    ASSERT_TRUE(vacancy.expirationDate.has_value());
    EXPECT_EQ(vacancy.expirationDate.value(), QDate(2025, 6, 1));
}
