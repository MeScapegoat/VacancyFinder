#pragma once
#include <QLoggingCategory>
#include <QMainWindow>


Q_DECLARE_LOGGING_CATEGORY(MainWindowLog);


class SharedContext;
class QSpinBox;
class QLineEdit;
class QPushButton;
class QButtonGroup;
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

    QButtonGroup& sortMode;

    QButtonGroup& filterMode;
    QLineEdit& filterLineEdit;

    QListView& vacanciesList;

    QLabel& titleLabel;
    QLabel& companyNameLabel;
    QLabel& levelLabel;
    QTextBrowser& vacancyDesc;

public slots:
    void updateVacanciesList();
    void clearList();

private:
    SharedContext* context = nullptr;

protected:
    void closeEvent(QCloseEvent*) override;
};