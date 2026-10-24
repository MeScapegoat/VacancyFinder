#pragma once

#include <VacancyExtractor.h>

#include <QString>
#include <QStringList>
#include <QUrl>


class LinkedInDetailsHtmlVacancyExtractor : public VacancyExtractor
{
public:
    LinkedInDetailsHtmlVacancyExtractor(const QString& html, const QUrl& url);
    ~LinkedInDetailsHtmlVacancyExtractor() = default;

protected:
    virtual QString title() const override;
    virtual QUrl url() const override;
    virtual QString companyName() const override;
    virtual QUrl companyUrl() const override;
    virtual QString location() const override;
    virtual QString employmentType() const override;
    virtual QString description() const override;
    virtual std::optional<QString> industry() const override;
    virtual std::optional<QString> level() const override;

private:
    const QString html;
    const QUrl vacancyUrl;

    QStringList jobCriteriaList;
};