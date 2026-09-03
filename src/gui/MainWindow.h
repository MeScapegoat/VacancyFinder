#pragma once
#include <QLoggingCategory>
#include <QMainWindow>


Q_DECLARE_LOGGING_CATEGORY(MainWindowLog);

// TODO: move to some .h or smth
enum class SortMode
{
    Off = 0,
    Relevance,

    Title,
    Company,

    Time,

    EmploymentType,

    Level,
    Experience,

    Education,
    Skills,

    Industry,

    ExpirationDate,


    COUNT
};

// TODO: move to some .h or smth
enum class FilterMode
{
    Off = 0,

    Include,
    Exclude,


    COUNT
};

// TODO: move to some .h or smth
enum class VacancyStatus
{
    Unreviewed = 0,

    Interested,
    Ignored,


    COUNT
};

class SharedContext;
class QSpinBox;
class QLineEdit;
class QPushButton;
class QComboBox;
class QListView;
class QLabel;
class QTextBrowser;
class QCheckBox;
class QStackedLayout;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(SharedContext* context);
    ~MainWindow();

public:
    QSpinBox& limitVacanciesInput;

    QLineEdit& keywordsLineEdit;
    QLineEdit& locationLineEdit;
    QPushButton& searchButton;

    QComboBox& sortMode;

    QComboBox& filterMode;
    QLineEdit& filterLineEdit;
    QCheckBox& unreviewedCheckBox;
    QCheckBox& ignoredCheckBox;
    QCheckBox& interestedCheckBox;

    QListView& vacanciesList;

    QLabel& titleLabel;
    QLabel& companyNameLabel;
    QLabel& employmentTypeLabel;
    QLabel& levelLabel;
    QLabel& experienceLabel;
    QLabel& educationLabel;
    QLabel& skillsLabel;
    QLabel& industryLabel;
    QLabel& expirationDateLabel;
    QTextBrowser& vacancyDesc;

    QStackedLayout& statusButtonsLayout;
    QPushButton& ignoreButton;
    QPushButton& interestButton;
    QPushButton& removeIgnoreButton;
    QPushButton& removeInterestButton;

public slots:
    void updateVacanciesList(const QStringList& data);
    void clearList();

private:
    SharedContext* context = nullptr;

protected:
    void closeEvent(QCloseEvent*) override;
};