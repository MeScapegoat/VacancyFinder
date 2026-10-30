#include "QueryParametersView.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSignalBlocker>
#include <QSpinBox>
#include <QString>

QueryParametersView::QueryParametersView(QWidget* parent) :
    View(parent),
    m_keywordsInput(*new QLineEdit),
    m_locationInput(*new QLineEdit),
    m_maxResultsInput(*new QSpinBox),
    m_searchButton(*new QPushButton),
    m_searchInterestedButton(*new QPushButton)
{
    QHBoxLayout& mainLayout = *new QHBoxLayout;


    QLabel& keywordsLabel = *new QLabel(tr("Keywords:"));
    QLabel& locationLabel = *new QLabel(tr("Location:"));
    QLabel& maxResultsLabel = *new QLabel(tr("Max amount of processed vacancies:"));
    m_maxResultsInput.setRange(1, 1000);
    m_searchButton.setText(tr("Start new search"));
    m_searchInterestedButton.setText(tr("Load interested"));
    m_searchInterestedButton.setVisible(false);


    mainLayout.addWidget(&keywordsLabel);
    mainLayout.addWidget(&m_keywordsInput);

    mainLayout.addWidget(&locationLabel);
    mainLayout.addWidget(&m_locationInput);

    mainLayout.addWidget(&maxResultsLabel);
    mainLayout.addWidget(&m_maxResultsInput);

    mainLayout.addWidget(&m_searchButton);
    mainLayout.addWidget(&m_searchInterestedButton);

    setLayout(&mainLayout);
}

const QLineEdit& QueryParametersView::keywordsInput() const
{
    return m_keywordsInput;
}

QLineEdit& QueryParametersView::keywordsInput()
{
    return m_keywordsInput;
}

const QLineEdit& QueryParametersView::locationInput() const
{
    return m_locationInput;
}

QLineEdit& QueryParametersView::locationInput()
{
    return m_locationInput;
}

const QSpinBox& QueryParametersView::maxResultsInput() const
{
    return m_maxResultsInput;
}

QSpinBox& QueryParametersView::maxResultsInput()
{
    return m_maxResultsInput;
}

const QPushButton& QueryParametersView::searchButton() const
{
    return m_searchButton;
}

QPushButton& QueryParametersView::searchButton()
{
    return m_searchButton;
}

const QPushButton& QueryParametersView::searchInterestedButton() const
{
    return m_searchInterestedButton;
}

QPushButton& QueryParametersView::searchInterestedButton()
{
    return m_searchInterestedButton;
}

void QueryParametersView::setKeywordsInput(const QString& keywords)
{
    m_keywordsInput.setText(keywords);
}

void QueryParametersView::setLocationInput(const QString& location)
{
    m_locationInput.setText(location);
}

void QueryParametersView::setMaxResultsInput(int maxResults)
{
    m_maxResultsInput.setValue(maxResults);
}
