#pragma once
// std

// Qt
#include <QMainWindow>
#include <QLoggingCategory>
#include <QListView>
#include <QWebEngineView>
#include <QLineEdit>
#include <QPushButton>

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
        QWebEngineView *descr() { return vacancyDesc; }
        QLineEdit *keywords() { return keywordsLineEdit; }
        QLineEdit *location() { return locationLineEdit; }

    public slots:
        void test();
        void appendList(const Vacancy &vacancy);

    protected:
        void closeEvent(QCloseEvent *) override;

        QLineEdit *keywordsLineEdit = nullptr;
        QLineEdit *locationLineEdit = nullptr;
        QPushButton *searchButton = nullptr;

        QListView *vacanciesList = nullptr;
        QWebEngineView *vacancyDesc = nullptr;
    };

} // namespace Linked