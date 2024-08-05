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

namespace Linked
{
    Q_DECLARE_LOGGING_CATEGORY(LinkedGUIWindow);

    class Window : public QMainWindow
    {
        Q_OBJECT

    public:
        Window();
        ~Window();

    public slots:
        void test();

    signals:
        void startSearch(const QString &keyword, const QString &location);
        void openVacancyDescription(const qsizetype &index);

    protected:
        void closeEvent(QCloseEvent *) override;

        QLineEdit *keywordsLineEdit = nullptr;
        QLineEdit *locationLineEdit = nullptr;
        QPushButton *searchButton = nullptr;

        QListView *vacanciesList = nullptr;
        QWebEngineView *vacancyDesc = nullptr;
    };

} // namespace Linked