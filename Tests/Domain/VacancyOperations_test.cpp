#include <gtest/gtest.h>
#include "VacancyOperations.h"
#include <Vacancy.h>
#include <utility>

namespace
{
    Vacancy makeVacancy(QString title = {}, QString company = {}, QDate date = QDate(2025, 1, 1))
    {
        Vacancy vacancy;
        vacancy.title = std::move(title);
        vacancy.companyName = std::move(company);
        vacancy.date = std::move(date);
        return vacancy;
    }
}

TEST(VacancyOperationsTest, ContainsAnyFilter)
{
    Vacancy vacancy = makeVacancy("C++ Qt Developer");

    EXPECT_TRUE(containsAnyFilter(QStringList{"Python", "Qt"}, &vacancy));
    EXPECT_FALSE(containsAnyFilter(QStringList{"Python", "Java"}, &vacancy));
    EXPECT_TRUE(containsAnyFilter(QStringList{}, &vacancy));
}

TEST(VacancyOperationsTest, ContainsAllFilter)
{
    Vacancy vacancy = makeVacancy("C++ Qt Developer");

    EXPECT_TRUE(containsAllFilter(QStringList{"C++", "Qt"}, &vacancy));
    EXPECT_FALSE(containsAllFilter(QStringList{"C++", "Python"}, &vacancy));
    EXPECT_TRUE(containsAllFilter(QStringList{}, &vacancy));
}

TEST(VacancyOperationsTest, ExcludesAnyFilter)
{
    Vacancy vacancy = makeVacancy("C++ Qt Developer");

    EXPECT_TRUE(excludesAnyFilter(QStringList{"Python", "Java"}, &vacancy));
    EXPECT_FALSE(excludesAnyFilter(QStringList{"Python", "Qt"}, &vacancy));
    EXPECT_TRUE(excludesAnyFilter(QStringList{}, &vacancy));
}

TEST(VacancyOperationsTest, ExcludesAllFilter)
{
    Vacancy vacancy = makeVacancy("C++ Qt Developer");

    EXPECT_TRUE(excludesAllFilter(QStringList{"Python", "Java"}, &vacancy));
    EXPECT_TRUE(excludesAllFilter(QStringList{"Python", "Qt"}, &vacancy));
    EXPECT_FALSE(excludesAllFilter(QStringList{"C++", "Qt"}, &vacancy));
    EXPECT_TRUE(excludesAllFilter(QStringList{}, &vacancy));
}

TEST(VacancyOperationsTest, RelevanceSort)
{
    Vacancy relevant = makeVacancy("Qt Qt Developer");
    Vacancy lessRelevant = makeVacancy("Qt Developer");

    EXPECT_TRUE(relevanceSort("Qt", &relevant, &lessRelevant));
    EXPECT_FALSE(relevanceSort("Qt", &lessRelevant, &relevant));
    EXPECT_FALSE(relevanceSort("Python", &relevant, &lessRelevant));
}

TEST(VacancyOperationsTest, TitleSort)
{
    Vacancy a = makeVacancy("AAAA");
    Vacancy b = makeVacancy("BBBB");

    EXPECT_TRUE(titleSort(&a, &b));
    EXPECT_FALSE(titleSort(&b, &a));
    EXPECT_FALSE(titleSort(&a, &a));
}

TEST(VacancyOperationsTest, CompanySort)
{
    Vacancy a = makeVacancy({}, "AAAA");
    Vacancy b = makeVacancy({}, "BBBB");

    EXPECT_TRUE(companySort(&a, &b));
    EXPECT_FALSE(companySort(&b, &a));
    EXPECT_FALSE(companySort(&a, &a));
}

TEST(VacancyOperationsTest, TimeSort)
{
    Vacancy older = makeVacancy({}, {}, QDate(2024, 1, 1));
    Vacancy newer = makeVacancy({}, {}, QDate(2025, 1, 1));

    EXPECT_TRUE(timeSort(&newer, &older));
    EXPECT_FALSE(timeSort(&older, &newer));
    EXPECT_FALSE(timeSort(&older, &older));
}

TEST(VacancyOperationsTest, EmploymentTypeSort)
{
    Vacancy a;
    Vacancy b;
    a.employmentType = "Full-time";
    b.employmentType = "Part-time";

    EXPECT_TRUE(employmentTypeSort(&b, &a));
    EXPECT_FALSE(employmentTypeSort(&a, &b));
    EXPECT_FALSE(employmentTypeSort(&a, &a));
}

TEST(VacancyOperationsTest, LevelSort)
{
    Vacancy a;
    Vacancy b;
    Vacancy missing;
    a.level = QString("AAAA");
    b.level = QString("BBBB");

    EXPECT_TRUE(levelSort(&a, &b));
    EXPECT_FALSE(levelSort(&b, &a));
    EXPECT_FALSE(levelSort(&missing, &b));
    EXPECT_TRUE(levelSort(&a, &missing));
    EXPECT_FALSE(levelSort(&missing, &missing));
}

TEST(VacancyOperationsTest, ExperienceSort)
{
    Vacancy junior;
    Vacancy senior;
    Vacancy missing;
    junior.experience = Months{12};
    senior.experience = Months{60};

    EXPECT_TRUE(experienceSort(&junior, &senior));
    EXPECT_FALSE(experienceSort(&senior, &junior));
    EXPECT_FALSE(experienceSort(&missing, &junior));
    EXPECT_TRUE(experienceSort(&junior, &missing));
    EXPECT_FALSE(experienceSort(&missing, &missing));
}

TEST(VacancyOperationsTest, EducationSort)
{
    Vacancy a;
    Vacancy b;
    Vacancy missing;
    a.education = QString("AAAA");
    b.education = QString("BBBB");

    EXPECT_TRUE(educationSort(&a, &b));
    EXPECT_FALSE(educationSort(&b, &a));
    EXPECT_FALSE(educationSort(&missing, &b));
    EXPECT_TRUE(educationSort(&a, &missing));
    EXPECT_FALSE(educationSort(&missing, &missing));
}

TEST(VacancyOperationsTest, SkillsSort)
{
    Vacancy empty;
    Vacancy oneSkill;
    Vacancy multipleSkills;
    oneSkill.skills = "C++";
    multipleSkills.skills = "C++; Qt";

    EXPECT_FALSE(skillsSort(&empty, &oneSkill));
    EXPECT_TRUE(skillsSort(&oneSkill, &empty));
    EXPECT_TRUE(skillsSort(&oneSkill, &multipleSkills));
    EXPECT_FALSE(skillsSort(&multipleSkills, &oneSkill));
    EXPECT_FALSE(skillsSort(&empty, &empty));
}

TEST(VacancyOperationsTest, IndustrySort)
{
    Vacancy finance;
    Vacancy technology;
    Vacancy missing;
    finance.industry = QString("Finance");
    technology.industry = QString("Technology");

    EXPECT_TRUE(industrySort(&finance, &technology));
    EXPECT_FALSE(industrySort(&technology, &finance));
    EXPECT_FALSE(industrySort(&missing, &finance));
    EXPECT_TRUE(industrySort(&finance, &missing));
    EXPECT_FALSE(industrySort(&missing, &missing));
}

TEST(VacancyOperationsTest, ExpirationDateSort)
{
    Vacancy earlier;
    Vacancy later;
    Vacancy missing;
    earlier.expirationDate = QDate(2025, 1, 1);
    later.expirationDate = QDate(2025, 6, 1);

    EXPECT_TRUE(expirationDateSort(&earlier, &later));
    EXPECT_FALSE(expirationDateSort(&later, &earlier));
    EXPECT_FALSE(expirationDateSort(&missing, &earlier));
    EXPECT_TRUE(expirationDateSort(&earlier, &missing));
    EXPECT_FALSE(expirationDateSort(&missing, &missing));
}
