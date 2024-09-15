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
#include "Vacancy.h"

namespace Linked
{
    Q_DECLARE_LOGGING_CATEGORY(LinkedGUIMainWindow);

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        MainWindow();
        ~MainWindow();

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
        void appendList(const Vacancy &vacancy);
        void clearList();

    protected:
        void closeEvent(QCloseEvent *) override;

        QLineEdit *keywordsLineEdit = nullptr;
        QLineEdit *locationLineEdit = nullptr;
        QPushButton *searchButton = nullptr;

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