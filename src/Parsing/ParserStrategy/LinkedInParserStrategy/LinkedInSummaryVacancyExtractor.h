#pragma once

#include <VacancyExtractor.h>

#include <QString>


class LinkedInSummaryVacancyExtractor : public VacancyExtractor
{
public:
    LinkedInSummaryVacancyExtractor() = default;
    ~LinkedInSummaryVacancyExtractor() = default;

public:
    void setHtml(const QString& html);

protected:
    virtual QString title() const override;
    virtual QUrl url() const override;
    virtual QString companyName() const override;
    virtual QUrl companyUrl() const override;
    virtual QString location() const override;
    virtual QDate date() const override;

private:
    QString html;
};

QStringList extractVacancySummaries(const QString& html);