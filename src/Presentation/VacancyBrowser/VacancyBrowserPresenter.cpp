#include "VacancyBrowserPresenter.h"

#include <QDate>
#include <QDebug>
#include <QLabel>
#include <QModelIndex>
#include <QStackedLayout>
#include <QString>
#include <QTextBrowser>
#include <QListView>
#include <QPushButton>
#include <QStringListModel>

#include "VacancyDetails.h"


Q_LOGGING_CATEGORY(VBPLog, "vacancyBrowser.presenter");


VacancyBrowserPresenter::VacancyBrowserPresenter(QObject* parent) :
    Presenter(parent)
{
    VacancyBrowserView& view = getView();

    QObject::connect(&view.vacancyList(), &QListView::clicked, this, &VacancyBrowserPresenter::openVacancyDetails);

    QObject::connect(&view.vacancyDetails().ignoreButton,     &QPushButton::released, this, &VacancyBrowserPresenter::onIgnoreButtonPressed);
    QObject::connect(&view.vacancyDetails().interestedButton, &QPushButton::released, this, &VacancyBrowserPresenter::onInterestButtonPressed);

    QObject::connect(&view.vacancyDetails().removeIgnoreButton,     &QPushButton::released, this, &VacancyBrowserPresenter::onRemoveIgnoreButtonPressed);
    QObject::connect(&view.vacancyDetails().removeInterestedButton, &QPushButton::released, this, &VacancyBrowserPresenter::onRemoveInterestButtonPressed);

    getView().vacancyList().setModel(&getModel().vacancyListModel());

    closeVacancyDetails();
}

void VacancyBrowserPresenter::openVacancyDetails(const QModelIndex& index)
{
    VacancyDetails& vacancyDetails = getView().vacancyDetails();
    const VacancyBrowserModel& model = getModel();

    const int id = index.row();
    const Vacancy& vacancy = *model.getVisibleVacancy(id);

    const VacancyStatus status = model.getVacancyStatus(vacancy);
    vacancyDetails.statusButtonsLayout.setCurrentIndex(static_cast<int>(status));

    const QString& title = vacancy.title;
    const QString& description = vacancy.description;
    const std::optional<QString>& level = vacancy.level;
    const std::optional<Months>& experience = vacancy.experience;
    const std::optional<QString>& industry = vacancy.industry;
    const std::optional<QString>& education = vacancy.education;
    const std::optional<QDate>& expirationDate = vacancy.expirationDate;
    const QString& skills = vacancy.skills;
    const QString& employmentType = vacancy.employmentType;
    const QString& companyName = vacancy.companyName;
    const QString& companyLink = vacancy.companyLink.toString();

    qCInfo(VBPLog) << "Loading descrption:" << id << "." << title;

    const QString linkPattern = "<a href='%1'>%2</a>";

    vacancyDetails.titleLabel.setText(linkPattern.arg(vacancy.link.toString()).arg(title));
    vacancyDetails.companyNameLabel.setText(linkPattern.arg(companyLink).arg(companyName));

    vacancyDetails.employmentTypeLabel.setText("Employment type: " + employmentType);


    vacancyDetails.levelLabel.setVisible(level.has_value());
    if (level) vacancyDetails.levelLabel.setText("Level: " + *level);

    vacancyDetails.experienceLabel.setVisible(experience.has_value());
    if (experience) vacancyDetails.experienceLabel.setText("Experience: " + QString::number(*experience / 12.0) + " years");

    vacancyDetails.educationLabel.setVisible(education.has_value());
    if (education) vacancyDetails.educationLabel.setText("Education: " + *education);

    vacancyDetails.skillsLabel.setVisible(!skills.isEmpty());
    if (!skills.isEmpty()) vacancyDetails.skillsLabel.setText("Skills: " + skills);

    vacancyDetails.industryLabel.setVisible(industry.has_value());
    if (industry) vacancyDetails.industryLabel.setText("Industry: " + *industry);

    vacancyDetails.expirationDateLabel.setVisible(expirationDate.has_value());
    if (expirationDate) vacancyDetails.expirationDateLabel.setText("Expiration date: " + expirationDate->toString("dd-MM-yyyy"));

    vacancyDetails.vacancyDesc.setHtml(description);
}

void VacancyBrowserPresenter::closeVacancyDetails()
{
    VacancyDetails& vacancyDetails = getView().vacancyDetails();

    vacancyDetails.titleLabel.setText("Title");
    vacancyDetails.companyNameLabel.setText("Company");

    vacancyDetails.employmentTypeLabel.setText("Employment type");

    vacancyDetails.levelLabel.setVisible(true);
    vacancyDetails.levelLabel.setText("Level");

    vacancyDetails.experienceLabel.setVisible(true);
    vacancyDetails.experienceLabel.setText("Experience");

    vacancyDetails.educationLabel.setVisible(true);
    vacancyDetails.educationLabel.setText("Education");

    vacancyDetails.skillsLabel.setVisible(true);
    vacancyDetails.skillsLabel.setText("Skills");

    vacancyDetails.industryLabel.setVisible(true);
    vacancyDetails.industryLabel.setText("Industry");

    vacancyDetails.expirationDateLabel.setVisible(true);
    vacancyDetails.expirationDateLabel.setText("Expiration date");


    vacancyDetails.vacancyDesc.setHtml(R"(
        <!DOCTYPE html>
        <html lang="en">
        <body>
            <header>
                <h1>Choose vacancy from left list to see its description here</h1>
            </header>
        </body>
        </html>)");

    vacancyDetails.statusButtonsLayout.setCurrentIndex(static_cast<int>(VacancyStatus::Unreviewed));
}

void VacancyBrowserPresenter::onIgnoreButtonPressed()
{
    VacancyBrowserModel& model = getModel();
    VacancyDetails& details = getView().vacancyDetails();

    const std::optional<constVacancyRef> vacancy = getCurrentVacancy();
    if (!vacancy)
        return;

    model.ignoredVacancies().insert(vacancy->get().link);

    updateVisibleVacancies();
    details.statusButtonsLayout.setCurrentIndex(static_cast<int>(VacancyStatus::Ignored));
    closeVacancyDetails();
}

void VacancyBrowserPresenter::onInterestButtonPressed()
{
    VacancyBrowserModel& model = getModel();
    VacancyDetails& details = getView().vacancyDetails();

    const std::optional<constVacancyRef> vacancy = getCurrentVacancy();
    if (!vacancy)
        return;

    model.interestedVacancies().insert(vacancy->get().link);

    updateVisibleVacancies();
    details.statusButtonsLayout.setCurrentIndex(static_cast<int>(VacancyStatus::Interested));
    closeVacancyDetails();
}

void VacancyBrowserPresenter::onRemoveIgnoreButtonPressed()
{
    VacancyBrowserModel& model = getModel();
    VacancyDetails& details = getView().vacancyDetails();

    const std::optional<constVacancyRef> vacancy = getCurrentVacancy();
    if (!vacancy)
        return;

    model.ignoredVacancies().remove(vacancy->get().link);

    updateVisibleVacancies();
    details.statusButtonsLayout.setCurrentIndex(static_cast<int>(VacancyStatus::Unreviewed));
    closeVacancyDetails();
}

void VacancyBrowserPresenter::onRemoveInterestButtonPressed()
{
    VacancyBrowserModel& model = getModel();
    VacancyDetails& details = getView().vacancyDetails();

    const std::optional<constVacancyRef> vacancy = getCurrentVacancy();
    if (!vacancy)
        return;

    model.interestedVacancies().remove(vacancy->get().link);

    updateVisibleVacancies();
    details.statusButtonsLayout.setCurrentIndex(static_cast<int>(VacancyStatus::Unreviewed));
    closeVacancyDetails();
}

void VacancyBrowserPresenter::onBriefReady(const Vacancy& vacancy)
{
    VacancyBrowserModel& model = getModel();
    auto& vacancyByUrl = model.vacancyByUrl();

    auto it = vacancyByUrl.find(vacancy.link);
    if (it == vacancyByUrl.end())
    {
        vacancyByUrl.emplace(vacancy.link, vacancy);
    }
    else
    {
        Vacancy& foundVacancy = it.value();
        foundVacancy.fillEmptyFrom(vacancy);

        qCDebug(VBPLog) << foundVacancy.title;
    }
}

void VacancyBrowserPresenter::onDetailsReady(const Vacancy& vacancy)
{
    VacancyBrowserModel& model = getModel();
    auto& vacancyByUrl = model.vacancyByUrl();

    auto it = vacancyByUrl.find(vacancy.link);
    if (it == vacancyByUrl.end())
    {
        vacancyByUrl.emplace(vacancy.link, vacancy);
    }
    else
    {
        Vacancy& foundVacancy = it.value();
        foundVacancy.fillEmptyFrom(vacancy);

        qCDebug(VBPLog) << foundVacancy.title;
    }
}

void VacancyBrowserPresenter::updateVacancyList(const QStringList& data)
{
    VacancyBrowserModel& model = getModel();

    model.vacancyListModel().setStringList(data);
}

void VacancyBrowserPresenter::clearVacancyList()
{
    VacancyBrowserModel& model = getModel();

    model.vacancyListModel().setStringList(QStringList());
}

void VacancyBrowserPresenter::updateVisibleVacancies()
{
    VacancyBrowserModel& model = getModel();

    //clearVacancyList();

    model.updateVisibleVacancies();

    updateModelData();
}

void VacancyBrowserPresenter::updateModelData()
{
    VacancyBrowserModel& model = getModel();

    QStringList data;

    for (const Vacancy* vacancy : model.visibleVacancies())
    {
        const QString& id = QString::number(data.size() + 1);
        const QString& title = vacancy->title;
        const QString& companyName = vacancy->companyName;
        const qsizetype& companyNameWidth = companyName.size() + 8;
        const QString& age = QString::number(vacancy->date.daysTo(QDate::currentDate()));
        const QString& location = vacancy->location;

        data.append(QString("%1. %2\n%3\n%4\n%5 days ago")
                    .arg(id.leftJustified(3))
                    .arg(title)
                    .arg(companyName)
                    .arg(location)
                    .arg(age));
    }

    updateVacancyList(data);
}

std::optional<VacancyBrowserPresenter::constVacancyRef> VacancyBrowserPresenter::getCurrentVacancy() const
{
    const QModelIndex& index = getView().vacancyList().currentIndex();
    const VacancyBrowserModel& model = getModel();

    if (!index.isValid())
        return std::nullopt;

    const int id = index.row();

    return *model.getVisibleVacancy(id);
}