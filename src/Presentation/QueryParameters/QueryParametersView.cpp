#include "QueryParametersView.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSignalBlocker>
#include <QSpinBox>

QueryParametersView::QueryParametersView(QWidget* parent) :
    View(parent),
    keywordsInput(*new QLineEdit),
    locationInput(*new QLineEdit),
    maxResultsInput(*new QSpinBox),
    searchButton(*new QPushButton),
    searchInterestedButton(*new QPushButton),
    resetButton(*new QPushButton)
{
    QHBoxLayout& mainLayout = *new QHBoxLayout;


    QLabel& keywordsLabel = *new QLabel(tr("Keywords:"));
    QLabel& locationLabel = *new QLabel(tr("Location:"));
    QLabel& maxResultsLabel = *new QLabel(tr("Max amount of processed vacancies:"));
    maxResultsInput.setRange(1, 999);
    searchButton.setText(tr("Search"));
    searchInterestedButton.setText(tr("Load interested"));
    resetButton.setText(tr("Reset"));


    mainLayout.addWidget(&keywordsLabel);
    mainLayout.addWidget(&keywordsInput);

    mainLayout.addWidget(&locationLabel);
    mainLayout.addWidget(&locationInput);

    mainLayout.addWidget(&maxResultsLabel);
    mainLayout.addWidget(&maxResultsInput);

    mainLayout.addWidget(&searchButton);
    mainLayout.addWidget(&searchInterestedButton);
    mainLayout.addWidget(&resetButton);

    setLayout(&mainLayout);
}
