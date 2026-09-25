#pragma once
#include <QLoggingCategory>
#include <QMainWindow>


Q_DECLARE_LOGGING_CATEGORY(MainWindowLog);

class QPushButton;
class QListView;
class QLabel;
class QTextBrowser;
class QStackedLayout;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget& queryParametersWidget, QWidget& resultViewOptionsWidget, QWidget* parent = nullptr);
    ~MainWindow();

public:
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

protected:
    void closeEvent(QCloseEvent*) override;
};