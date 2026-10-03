#pragma once

#include <VacancyExtractor.h>

#include <QString>


class LinkedInBriefVacancyExtractor : public VacancyExtractor
{
public:
    LinkedInBriefVacancyExtractor() = default;
    ~LinkedInBriefVacancyExtractor() = default;

public:
    void setHtml(const QString& html);

protected:
    virtual QString title() const override;
    virtual QUrl link() const override;
    virtual QString companyName() const override;
    virtual QUrl companyLink() const override;
    virtual QString location() const override;
    virtual QDate date() const override;

private:
    QString html;
};

QStringList extractVacancyBriefs(const QString& html);