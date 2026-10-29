#include "VacancyBrowserModel.h"

#include <gtest/gtest.h>

#include <QSet>
#include <QStringListModel>

#include <QtTestPrinters.h>


Vacancy makeVacancy(QString url = "url",
                    QString title = "title",
                    QString company = "company",
                    QString location = "location",
                    QDate date = QDate::currentDate())
{
    Vacancy vacancy;
    vacancy.url = QUrl(url);
    vacancy.title = std::move(title);
    vacancy.companyName = std::move(company);
    vacancy.location = std::move(location);
    vacancy.date = date;
    return vacancy;
}

class VacancyBrowserModelTest : public ::testing::Test
{
protected:
    VacancyBrowserModel model;

    void SetUp() override
    {
        model.setVisibleStatuses({VacancyStatus::Unreviewed, VacancyStatus::Ignored, VacancyStatus::Interested});
        model.setIgnoredCompaniesVisible(false);
        model.setVacancyFilter([](const Vacancy*)
        {
            return true;
        });
    }
};

TEST_F(VacancyBrowserModelTest, StartsWithEmptyCollections)
{
    EXPECT_TRUE(model.vacancyByUrl().isEmpty());
    EXPECT_TRUE(model.visibleVacancies().isEmpty());
    EXPECT_TRUE(model.ignoredVacancies().isEmpty());
    EXPECT_TRUE(model.ignoredCompanies().isEmpty());
    EXPECT_TRUE(model.interestedVacancies().isEmpty());
    EXPECT_EQ(model.vacancyListModel().rowCount(), 0);
}

TEST_F(VacancyBrowserModelTest, AddVacancyStoresItByUrl)
{
    const Vacancy vacancy = makeVacancy();
    model.addVacancy(vacancy);
    ASSERT_EQ(model.vacancyByUrl().size(), 1);
    EXPECT_TRUE(model.vacancyByUrl().contains(vacancy.url));
    EXPECT_EQ(model.vacancyByUrl().value(vacancy.url).title, vacancy.title);
}

TEST_F(VacancyBrowserModelTest, AddingSameUrlDoesNotCreateDuplicate)
{
    const Vacancy first = makeVacancy();
    const Vacancy second = first;
    model.addVacancy(first);
    model.addVacancy(second);
    ASSERT_EQ(model.vacancyByUrl().size(), 1);
    EXPECT_EQ(model.vacancyByUrl().value(first.url).title, first.title);
}

TEST_F(VacancyBrowserModelTest, AddingSameUrlFillsMissingVacancyDetails)
{
    const Vacancy summary = makeVacancy("url", "Developer", "First Company", QString());

    Vacancy details{};
    details.url = summary.url;
    details.title = "Updated title";
    details.companyName = "Updated company";
    details.location = "Budapest";
    details.description = "Full description";
    details.level = "Senior";

    model.addVacancy(summary);
    model.addVacancy(details);

    const Vacancy& merged = model.vacancyByUrl().constFind(summary.url).value();
    EXPECT_EQ(merged.title, summary.title);
    EXPECT_EQ(merged.companyName, summary.companyName);
    EXPECT_EQ(merged.location, details.location);
    EXPECT_EQ(merged.description, details.description);
    ASSERT_TRUE(merged.level.has_value());
    EXPECT_EQ(*merged.level, *details.level);
}

TEST_F(VacancyBrowserModelTest, NewVacancyIsUnreviewed)
{
    const Vacancy vacancy = makeVacancy();
    EXPECT_EQ(model.getVacancyStatus(vacancy), VacancyStatus::Unreviewed);
}

TEST_F(VacancyBrowserModelTest, IgnoredVacancyHasIgnoredStatus)
{
    const Vacancy vacancy = makeVacancy();
    model.ignoredVacancies().insert(vacancy.url);
    EXPECT_EQ(model.getVacancyStatus(vacancy), VacancyStatus::Ignored);
}

TEST_F(VacancyBrowserModelTest, InterestedVacancyHasInterestedStatus)
{
    const Vacancy vacancy = makeVacancy();
    model.interestedVacancies().insert(vacancy.url);
    EXPECT_EQ(model.getVacancyStatus(vacancy), VacancyStatus::Interested);
}

TEST_F(VacancyBrowserModelTest, SetIgnoredVacanciesReplacesSet)
{
    const QUrl first("url1"), second("url2");

    model.ignoredVacancies().insert(first);
    model.setIgnoredVacancies(QSet<QUrl>{second});

    EXPECT_FALSE(model.ignoredVacancies().contains(first));
    EXPECT_TRUE(model.ignoredVacancies().contains(second));
}

TEST_F(VacancyBrowserModelTest, IgnoredCompanyVisibilityIsSeparateFromIgnoredVacancyStatus)
{
    Vacancy ignoredCompanyVacancy = makeVacancy("company-url", "Role", "Intel");
    ignoredCompanyVacancy.companyUrl = QUrl("https://www.linkedin.com/company/intel-corporation");

    Vacancy sameNameDifferentCompany = makeVacancy("other-company-url", "Other role", "Intel");
    sameNameDifferentCompany.companyUrl = QUrl("https://www.linkedin.com/company/intel-corporation-2");

    const Vacancy ignoredVacancy = makeVacancy("ignored-vacancy-url", "Other role", "Other company");

    model.addVacancy(ignoredCompanyVacancy);
    model.addVacancy(sameNameDifferentCompany);
    model.addVacancy(ignoredVacancy);
    model.ignoredCompanies().insert(ignoredCompanyVacancy.companyUrl);
    model.ignoredVacancies().insert(ignoredVacancy.url);
    model.setVisibleStatuses({VacancyStatus::Unreviewed});

    model.updateVisibleVacancies();

    ASSERT_EQ(model.visibleVacancies().size(), 1);
    EXPECT_EQ(model.visibleVacancies().first()->url, sameNameDifferentCompany.url);

    model.setIgnoredCompaniesVisible(true);
    model.updateVisibleVacancies();

    ASSERT_EQ(model.visibleVacancies().size(), 2);
    QSet<QUrl> visibleUrls;
    for (const Vacancy* vacancy : model.visibleVacancies())
    {
        visibleUrls.insert(vacancy->url);
    }
    EXPECT_TRUE(visibleUrls.contains(ignoredCompanyVacancy.url));
    EXPECT_TRUE(visibleUrls.contains(sameNameDifferentCompany.url));
    EXPECT_FALSE(visibleUrls.contains(ignoredVacancy.url));
}

TEST_F(VacancyBrowserModelTest, IgnoredCompanyFilterRunsBeforeWordFilter)
{
    Vacancy vacancy = makeVacancy("company-vacancy-url");
    vacancy.companyUrl = QUrl("https://www.linkedin.com/company/ignored-company");
    model.addVacancy(vacancy);
    model.ignoredCompanies().insert(vacancy.companyUrl);

    bool wordFilterCalled = false;
    model.setVacancyFilter([&wordFilterCalled](const Vacancy*)
    {
        wordFilterCalled = true;
        return true;
    });

    model.updateVisibleVacancies();

    EXPECT_TRUE(model.visibleVacancies().isEmpty());
    EXPECT_FALSE(wordFilterCalled);
}

TEST_F(VacancyBrowserModelTest, SetInterestedVacanciesReplacesSet)
{
    const QUrl first("url1"), second("url2");

    model.interestedVacancies().insert(first);
    model.setInterestedVacancies(QSet<QUrl>{second});

    EXPECT_FALSE(model.interestedVacancies().contains(first));
    EXPECT_TRUE(model.interestedVacancies().contains(second));
}

TEST_F(VacancyBrowserModelTest, UpdateMakesAllPassingVacanciesVisible)
{
    model.addVacancy(makeVacancy("url1"));
    model.addVacancy(makeVacancy("url2"));
    model.updateVisibleVacancies();
    EXPECT_EQ(model.visibleVacancies().size(), 2);
    EXPECT_EQ(model.vacancyListModel().rowCount(), 2);
}

TEST_F(VacancyBrowserModelTest, StatusFilterRemovesIgnoredVacancies)
{
    const Vacancy ignored =    makeVacancy("url1");
    const Vacancy unreviewed = makeVacancy("url2");

    model.addVacancy(ignored);
    model.addVacancy(unreviewed);

    model.ignoredVacancies().insert(ignored.url);
    model.setVisibleStatuses({VacancyStatus::Unreviewed});

    model.updateVisibleVacancies();

    ASSERT_EQ(model.visibleVacancies().size(), 1);
    EXPECT_EQ(model.visibleVacancies().first()->url, unreviewed.url);
}

TEST_F(VacancyBrowserModelTest, EmptyVisibleStatusesHideAllVacancies)
{
    model.addVacancy(makeVacancy());
    model.setVisibleStatuses({});
    model.updateVisibleVacancies();

    EXPECT_TRUE(model.visibleVacancies().isEmpty());
    EXPECT_EQ(model.vacancyListModel().rowCount(), 0);
}

TEST_F(VacancyBrowserModelTest, GetVisibleVacancyReturnsValidReference)
{
    const Vacancy vacancy = makeVacancy();

    model.addVacancy(vacancy);

    model.updateVisibleVacancies();

    const auto result = model.getVisibleVacancy(0);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->get().title, vacancy.title);
}

TEST_F(VacancyBrowserModelTest, GetVisibleVacancyReturnsEmptyForInvalidIndex)
{
    model.addVacancy(makeVacancy());
    model.updateVisibleVacancies();

    EXPECT_FALSE(model.getVisibleVacancy(1).has_value());
    EXPECT_FALSE(model.getVisibleVacancy(100).has_value());
}

TEST_F(VacancyBrowserModelTest, RefreshRebuildsVisibleList)
{
    model.addVacancy(makeVacancy("url1"));
    model.updateVisibleVacancies();
    model.addVacancy(makeVacancy("url2"));

    EXPECT_EQ(model.vacancyListModel().rowCount(), 1);

    model.updateVisibleVacancies();

    EXPECT_EQ(model.vacancyListModel().rowCount(), 2);
}

TEST_F(VacancyBrowserModelTest, AppliesAssignedFilter)
{
    const Vacancy qtVacancy = makeVacancy("url1", "Qt Developer");
    const Vacancy javaVacancy = makeVacancy("url2", "Java Developer");

    model.addVacancy(qtVacancy);
    model.addVacancy(javaVacancy);
    model.setVacancyFilter([](const Vacancy* v)
    {
        return v->title.contains("Qt");
    });

    model.updateVisibleVacancies();

    ASSERT_EQ(model.visibleVacancies().size(), 1);
    EXPECT_EQ(model.visibleVacancies().first()->title, qtVacancy.title);
}

TEST_F(VacancyBrowserModelTest, EmptyFilterSkipsWordFilteringButStillAppliesStatusFilter)
{
    const Vacancy unreviewedVacancy = makeVacancy("url1", "Unreviewed");
    const Vacancy ignoredVacancy = makeVacancy("url2", "Ignored");

    model.addVacancy(unreviewedVacancy);
    model.addVacancy(ignoredVacancy);
    model.ignoredVacancies().insert(ignoredVacancy.url);
    model.setVisibleStatuses({VacancyStatus::Unreviewed});
    model.setVacancyFilter(VacancyFilter{});

    model.updateVisibleVacancies();

    ASSERT_EQ(model.visibleVacancies().size(), 1);
    EXPECT_EQ(model.visibleVacancies().first()->title, unreviewedVacancy.title);

    model.setVacancyFilter([](const Vacancy*)
    {
        return true;
    });
    model.updateVisibleVacancies();

    ASSERT_EQ(model.visibleVacancies().size(), 1);
    EXPECT_EQ(model.visibleVacancies().first()->title, unreviewedVacancy.title);
}

TEST_F(VacancyBrowserModelTest, AppliesAssignedSortComparator)
{
    const Vacancy vacancy1= makeVacancy("url1", "AAAA");
    const Vacancy vacancy2 = makeVacancy("url2", "ZZZZ");
    model.addVacancy(vacancy1);
    model.addVacancy(vacancy2);
    model.setVacancySortComparator([](const Vacancy* a, const Vacancy* b)
    {
        return a->title > b->title;
    });

    model.updateVisibleVacancies();

    ASSERT_EQ(model.visibleVacancies().size(), 2);
    EXPECT_EQ(model.visibleVacancies()[0]->title, vacancy2.title);
    EXPECT_EQ(model.visibleVacancies()[1]->title, vacancy1.title);
}
