#pragma once
// std

// Qt
#include <QMainWindow>
#include <QLoggingCategory>
#include <QListView>
#include <QTextBrowser>
#include <QLineEdit>
#include <QPushButton>
#include <QButtonGroup>
#include <QLabel>

// custom
#include "../core/SharedContext.h"
#include "../core/Vacancy.h"

namespace Linked
{
    Q_DECLARE_LOGGING_CATEGORY(LinkedGUIMainWindow);

    class SharedContext;

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        MainWindow(SharedContext *context);
        ~MainWindow();

    public:
        int maxVacancies() const { return limitLineEdit->text().toInt(); }

    public:
        QListView *list() { return vacanciesList; }
        QPushButton *button() { return searchButton; }
        QTextBrowser *descr() { return vacancyDesc; }
        QLineEdit *keywords() { return keywordsLineEdit; }
        QLineEdit *location() { return locationLineEdit; }
        QButtonGroup *sortMode() { return sortMode_; }
        QButtonGroup *filterMode() { return filterMode_; }
        QLineEdit *filterLineEdit() { return filterLineEdit_; }
        QLabel *titleLabel() { return titleLabel_; }
        QLabel *companyNameLabel() { return companyNameLabel_; }
        QLabel *levelLabel() { return levelLabel_; }

    public slots:
        void updateVacanciesList();
        void clearList();

    private:
        SharedContext *context = nullptr;

    protected:
        void closeEvent(QCloseEvent *) override;

        QLineEdit *keywordsLineEdit = nullptr;
        QLineEdit *locationLineEdit = nullptr;
        QPushButton *searchButton = nullptr;

        QLineEdit *limitLineEdit = nullptr;

        QButtonGroup *sortMode_ = nullptr;

        QButtonGroup *filterMode_ = nullptr;
        QLineEdit *filterLineEdit_ = nullptr;

        QListView *vacanciesList = nullptr;

        QLabel *titleLabel_ = nullptr;
        QLabel *companyNameLabel_ = nullptr;
        QLabel *levelLabel_ = nullptr;
        QTextBrowser *vacancyDesc = nullptr;
    };

} // namespace Linked