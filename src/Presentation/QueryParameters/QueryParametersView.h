#pragma once

#include <View.h>

#include <QWidget>
#include <QObject>

class QLineEdit;
class QSpinBox;
class QPushButton;
class QString;

class QueryParametersView : public View
{
public:
    QueryParametersView(QWidget* parent = nullptr);
    ~QueryParametersView() = default;

public:
    const QLineEdit& keywordsInput() const;
    QLineEdit& keywordsInput();

    const QLineEdit& locationInput() const;
    QLineEdit& locationInput();

    const QSpinBox& maxResultsInput() const;
    QSpinBox& maxResultsInput();


    const QPushButton& searchButton() const;
    QPushButton& searchButton();

    const QPushButton& searchInterestedButton() const;
    QPushButton& searchInterestedButton();

    const QPushButton& resetButton() const;
    QPushButton& resetButton();

    void setKeywordsInput(const QString& keywords);
    void setLocationInput(const QString& location);
    void setMaxResultsInput(int maxResults);

private:
    QLineEdit& m_keywordsInput;
    QLineEdit& m_locationInput;
    QSpinBox& m_maxResultsInput;

    QPushButton& m_searchButton;
    QPushButton& m_searchInterestedButton;
    QPushButton& m_resetButton;
};
