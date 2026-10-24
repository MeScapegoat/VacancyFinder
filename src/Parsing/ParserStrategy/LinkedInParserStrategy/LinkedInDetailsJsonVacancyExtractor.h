#pragma once

#include <VacancyExtractor.h>

#include <QJsonObject>
#include <QString>


class LinkedInDetailsJsonVacancyExtractor : public VacancyExtractor
{
public:
    LinkedInDetailsJsonVacancyExtractor(const QJsonObject& json, const QUrl& url);
    ~LinkedInDetailsJsonVacancyExtractor() = default;

protected:
    virtual QString title() const override;
    virtual QUrl url() const override;
    virtual QString companyName() const override;
    virtual QUrl companyUrl() const override;
    virtual QString location() const override;
    virtual QString employmentType() const override;
    virtual QString skills() const override;
    virtual QString description() const override;
    virtual QDate date() const override;
    virtual std::optional<QString> industry() const override;
    virtual std::optional<Months> experience() const override;
    virtual std::optional<QString> education() const override;
    virtual std::optional<QDate> expirationDate() const override;

private:
    const QJsonObject json;
    const QUrl vacancyUrl;

    std::optional<QJsonObject> organization;
};

QJsonObject extractJsonObject(const QString& html);