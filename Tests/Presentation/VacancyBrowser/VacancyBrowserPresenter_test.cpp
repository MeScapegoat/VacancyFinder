#include <gtest/gtest.h>

#include <QUrl>
#include <QListView>
#include <QStringListModel>

#include "VacancyBrowserPresenter.h"
#include "VacancyBrowserModel.h"
#include "VacancyBrowserView.h"

class VacancyBrowserPresenterTest : public testing::Test, public VacancyBrowserPresenter
{
protected:
    VacancyBrowserPresenterTest()
    {
        setVisibleStatuses({VacancyStatus::Unreviewed, VacancyStatus::Ignored, VacancyStatus::Interested});
        setVacancyFilter([](const Vacancy*)
        {
            return true;
        });
    }
    static Vacancy makeVacancy(const QString& title, const QUrl& link)
    {
        Vacancy vacancy{};
        vacancy.title = title;
        vacancy.link = link;
        return vacancy;
    }
};

TEST_F(VacancyBrowserPresenterTest, ConstructorAssignsModelToVacancyList)
{
    EXPECT_EQ(view().vacancyList().model(), &model().vacancyListModel());
}

TEST_F(VacancyBrowserPresenterTest, IgnoredVacanciesGetterAndSetter)
{
    const QSet<QUrl> expected{
        QUrl("url1"),
        QUrl("url2")
    };

    setIgnoredVacancies(expected);
    EXPECT_EQ(ignoredVacancies(), expected);

    setIgnoredVacancies({});
    EXPECT_TRUE(ignoredVacancies().isEmpty());
}

TEST_F(VacancyBrowserPresenterTest, InterestedVacanciesGetterAndSetter)
{
    const QSet<QUrl> expected{
        QUrl("url1"),
        QUrl("url2")
    };

    setInterestedVacancies(expected);
    EXPECT_EQ(interestedVacancies(), expected);

    setInterestedVacancies({});
    EXPECT_TRUE(interestedVacancies().isEmpty());
}

TEST_F(VacancyBrowserPresenterTest, UpdateVisibleVacanciesUpdatesListModel)
{
    const Vacancy vacancy = makeVacancy("Developer", QUrl("url"));
    model().addVacancy(vacancy);

    updateVisibleVacancies();

    EXPECT_EQ(view().vacancyList().model()->rowCount(), 1);
}

TEST_F(VacancyBrowserPresenterTest, ResetClearsWorkflowAndPreservesMetadata)
{
    const QUrl vacancyUrl("current");
    const QUrl ignoredUrl("ignored");
    const QUrl interestedUrl("interested");

    model().addVacancy(makeVacancy("Current", vacancyUrl));
    updateVisibleVacancies();

    setIgnoredVacancies({ignoredUrl});
    setInterestedVacancies({interestedUrl});

    reset();

    EXPECT_TRUE(model().vacancyByUrl().isEmpty());
    EXPECT_TRUE(model().visibleVacancies().isEmpty());
    EXPECT_EQ(ignoredVacancies(), QSet<QUrl>{ignoredUrl});
    EXPECT_EQ(interestedVacancies(), QSet<QUrl>{interestedUrl});
}