#include <gtest/gtest.h>

#include <QItemSelectionModel>
#include <QLabel>
#include <QList>
#include <QListView>
#include <QModelIndex>
#include <QPushButton>
#include <QSet>
#include <QSignalBlocker>
#include <QString>
#include <QStringListModel>
#include <QTextBrowser>
#include <QUrl>

#include <Vacancy.h>
#include "VacancyBrowserModel.h"
#include "VacancyBrowserPresenter.h"
#include "VacancyBrowserView.h"
#include "VacancyDetails.h"
#include <VacancyStatus.h>

class VacancyBrowserPresenterTest : public testing::Test, public VacancyBrowserPresenter
{
protected:
    VacancyBrowserPresenterTest()
    {
        setVisibleStatuses({VacancyStatus::Unreviewed, VacancyStatus::Ignored, VacancyStatus::Interested});
        setIgnoredCompaniesVisible(false);
        setVacancyFilter([](const Vacancy*)
        {
            return true;
        });
    }
    static Vacancy makeVacancy(const QString& title, const QUrl& url)
    {
        Vacancy vacancy{};
        vacancy.title = title;
        vacancy.url = url;
        return vacancy;
    }

    void selectFirstVacancy()
    {
        const QModelIndex index = model().vacancyListModel().index(0, 0);
        view().vacancyList().listView().setCurrentIndex(index);
    }

    QPushButton* buttonWithText(const QString& text)
    {
        for (QPushButton* button : view().vacancyDetails().findChildren<QPushButton*>())
        {
            if (button->text() == text)
            {
                return button;
            }
        }

        return nullptr;
    }
};

TEST_F(VacancyBrowserPresenterTest, ConstructorAssignsModelToVacancyList)
{
    EXPECT_EQ(view().vacancyList().listView().model(), &model().vacancyListModel());
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

TEST_F(VacancyBrowserPresenterTest, CompanyIgnoreActionDoesNotChangeIgnoredVacancies)
{
    Vacancy vacancy = makeVacancy("Developer", QUrl("developer"));
    vacancy.companyName = "Acme";
    vacancy.companyUrl = QUrl("https://www.linkedin.com/company/acme");
    onVacanciesReady({vacancy});
    selectFirstVacancy();

    QPushButton* ignoreCompanyButton = buttonWithText("Ignore company");
    ASSERT_NE(ignoreCompanyButton, nullptr);
    ignoreCompanyButton->click();

    const QUrl companyUrl = vacancy.companyUrl;
    EXPECT_TRUE(ignoredCompanies().contains(companyUrl));
    EXPECT_TRUE(ignoredVacancies().isEmpty());
    EXPECT_TRUE(model().visibleVacancies().isEmpty());

    setIgnoredCompaniesVisible(true);
    ASSERT_EQ(model().visibleVacancies().size(), 1);
    selectFirstVacancy();

    QPushButton* removeCompanyIgnoreButton = buttonWithText("Remove company from ignored list");
    ASSERT_NE(removeCompanyIgnoreButton, nullptr);
    removeCompanyIgnoreButton->click();

    EXPECT_FALSE(ignoredCompanies().contains(companyUrl));
    EXPECT_TRUE(ignoredVacancies().isEmpty());
    ASSERT_EQ(model().visibleVacancies().size(), 1);
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

    EXPECT_EQ(view().vacancyList().listView().model()->rowCount(), 1);
}

TEST_F(VacancyBrowserPresenterTest, VacanciesReadyPopulatesVisibleList)
{
    const QList<Vacancy> vacancies{
        makeVacancy("Developer", QUrl("developer")),
        makeVacancy("Designer", QUrl("designer"))
    };

    onVacanciesReady(vacancies);

    EXPECT_EQ(model().vacancyByUrl().size(), 2);
    EXPECT_EQ(model().visibleVacancies().size(), 2);
    EXPECT_EQ(view().vacancyList().listView().model()->rowCount(), 2);

    QLabel* visibleVacancyCount = view().vacancyList().findChild<QLabel*>("visibleVacancyCount");
    ASSERT_NE(visibleVacancyCount, nullptr);
    EXPECT_EQ(visibleVacancyCount->text(), QStringLiteral("Visible vacancies: 2"));
}

TEST_F(VacancyBrowserPresenterTest, VisibleVacancyCountTracksFilteredVacancies)
{
    setVacancyFilter([](const Vacancy* vacancy)
    {
        return vacancy->title == QStringLiteral("Visible");
    });
    onVacanciesReady({
        makeVacancy("Visible", QUrl("visible")),
        makeVacancy("Hidden", QUrl("hidden"))
    });

    QLabel* visibleVacancyCount = view().vacancyList().findChild<QLabel*>("visibleVacancyCount");
    ASSERT_NE(visibleVacancyCount, nullptr);
    EXPECT_EQ(visibleVacancyCount->text(), QStringLiteral("Visible vacancies: 1"));

    setVacancyFilter([](const Vacancy*)
    {
        return true;
    });
    updateVisibleVacancies();
    EXPECT_EQ(visibleVacancyCount->text(), QStringLiteral("Visible vacancies: 2"));
}

TEST_F(VacancyBrowserPresenterTest, VisibleVacancyCountMatchesHeadingFont)
{
    QLabel* heading = view().vacancyList().findChild<QLabel*>("vacancyListHeading");
    QLabel* visibleVacancyCount = view().vacancyList().findChild<QLabel*>("visibleVacancyCount");
    ASSERT_NE(heading, nullptr);
    ASSERT_NE(visibleVacancyCount, nullptr);

    EXPECT_EQ(visibleVacancyCount->font().pointSize(), heading->font().pointSize());
    EXPECT_EQ(visibleVacancyCount->font().weight(), heading->font().weight());
}

TEST_F(VacancyBrowserPresenterTest, SelectingVacancyDisplaysItsDescription)
{
    Vacancy vacancy = makeVacancy("Developer", QUrl("developer"));
    vacancy.description = "Build useful tools.";
    onVacanciesReady({vacancy});

    selectFirstVacancy();

    QTextBrowser* description = view().vacancyDetails().findChild<QTextBrowser*>();
    ASSERT_NE(description, nullptr);
    EXPECT_EQ(description->toPlainText(), vacancy.description);
}

TEST_F(VacancyBrowserPresenterTest, ChangingCurrentSelectionUpdatesDisplayedDetails)
{
    Vacancy firstVacancy = makeVacancy("First", QUrl("first"));
    firstVacancy.description = "First description.";
    Vacancy secondVacancy = makeVacancy("Second", QUrl("second"));
    secondVacancy.description = "Second description.";
    onVacanciesReady({firstVacancy, secondVacancy});
    const Vacancy* selectedVacancy = model().visibleVacancies()[1];

    selectFirstVacancy();
    view().vacancyList().listView().setCurrentIndex(model().vacancyListModel().index(1, 0));

    QTextBrowser* description = view().vacancyDetails().findChild<QTextBrowser*>();
    ASSERT_NE(description, nullptr);
    EXPECT_EQ(description->toPlainText(), selectedVacancy->description);
}

TEST_F(VacancyBrowserPresenterTest, StatusActionsUpdateVacancyMetadata)
{
    const Vacancy vacancy = makeVacancy("Developer", QUrl("developer"));
    onVacanciesReady({vacancy});

    selectFirstVacancy();
    QPushButton* ignoreButton = buttonWithText("Not interested");
    ASSERT_NE(ignoreButton, nullptr);
    ignoreButton->click();
    EXPECT_TRUE(ignoredVacancies().contains(vacancy.url));
    EXPECT_EQ(model().getVacancyStatus(vacancy), VacancyStatus::Ignored);

    selectFirstVacancy();
    QPushButton* removeIgnoreButton = buttonWithText("Remove from not interested list");
    ASSERT_NE(removeIgnoreButton, nullptr);
    removeIgnoreButton->click();
    EXPECT_FALSE(ignoredVacancies().contains(vacancy.url));
    EXPECT_EQ(model().getVacancyStatus(vacancy), VacancyStatus::Unreviewed);

    selectFirstVacancy();
    QPushButton* interestedButton = buttonWithText("Interested");
    ASSERT_NE(interestedButton, nullptr);
    interestedButton->click();
    EXPECT_TRUE(interestedVacancies().contains(vacancy.url));
    EXPECT_EQ(model().getVacancyStatus(vacancy), VacancyStatus::Interested);

    selectFirstVacancy();
    QPushButton* removeInterestedButton = buttonWithText("Remove from interested list");
    ASSERT_NE(removeInterestedButton, nullptr);
    removeInterestedButton->click();
    EXPECT_FALSE(interestedVacancies().contains(vacancy.url));
    EXPECT_EQ(model().getVacancyStatus(vacancy), VacancyStatus::Unreviewed);
}

TEST_F(VacancyBrowserPresenterTest, StatusActionUsesVacancyShownInDetails)
{
    Vacancy firstVacancy = makeVacancy("First", QUrl("first"));
    firstVacancy.description = "First description.";
    Vacancy secondVacancy = makeVacancy("Second", QUrl("second"));
    secondVacancy.description = "Second description.";
    onVacanciesReady({firstVacancy, secondVacancy});
    const QUrl displayedVacancyUrl = model().visibleVacancies()[0]->url;
    const QUrl selectedVacancyUrl = model().visibleVacancies()[1]->url;
    const QString displayedDescription = model().visibleVacancies()[0]->description;

    selectFirstVacancy();
    QPushButton* interestedButton = buttonWithText("Interested");
    ASSERT_NE(interestedButton, nullptr);

    {
        const QSignalBlocker blocker(view().vacancyList().listView().selectionModel());
        view().vacancyList().listView().setCurrentIndex(model().vacancyListModel().index(1, 0));

        QTextBrowser* description = view().vacancyDetails().findChild<QTextBrowser*>();
        ASSERT_NE(description, nullptr);
        EXPECT_EQ(description->toPlainText(), displayedDescription);

        interestedButton->click();
    }

    EXPECT_TRUE(interestedVacancies().contains(displayedVacancyUrl));
    EXPECT_FALSE(interestedVacancies().contains(selectedVacancyUrl));
}

TEST_F(VacancyBrowserPresenterTest, StatusActionsWithoutSelectionLeaveMetadataUnchanged)
{
    const QUrl ignoredUrl("ignored");
    const QUrl interestedUrl("interested");
    setIgnoredVacancies({ignoredUrl});
    setInterestedVacancies({interestedUrl});

    QPushButton* ignoreButton = buttonWithText("Not interested");
    QPushButton* interestedButton = buttonWithText("Interested");
    ASSERT_NE(ignoreButton, nullptr);
    ASSERT_NE(interestedButton, nullptr);
    ignoreButton->click();
    interestedButton->click();

    EXPECT_EQ(ignoredVacancies(), QSet<QUrl>{ignoredUrl});
    EXPECT_EQ(interestedVacancies(), QSet<QUrl>{interestedUrl});
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
    QLabel* visibleVacancyCount = view().vacancyList().findChild<QLabel*>("visibleVacancyCount");
    ASSERT_NE(visibleVacancyCount, nullptr);
    EXPECT_EQ(visibleVacancyCount->text(), QStringLiteral("Visible vacancies: 0"));
    EXPECT_EQ(ignoredVacancies(), QSet<QUrl>{ignoredUrl});
    EXPECT_EQ(interestedVacancies(), QSet<QUrl>{interestedUrl});
}
