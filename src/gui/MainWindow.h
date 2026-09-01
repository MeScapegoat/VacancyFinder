#pragma once
#include <QLoggingCategory>
#include <QMainWindow>


Q_DECLARE_LOGGING_CATEGORY(MainWindowLog);


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

enum class FilterMode
{
    Off = 0,

    Include,
    Exclude,


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

public slots:
    void updateVacanciesList();
    void clearList();

private:
    SharedContext* context = nullptr;

protected:
    void closeEvent(QCloseEvent*) override;
};