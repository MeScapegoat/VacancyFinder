// std

// Qt
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>
#include <QNetworkReply>

class LinkedApp : public QObject
{
    Q_OBJECT

public:
    LinkedApp(QObject* parent = nullptr);
    ~LinkedApp() = default;

public:
    void sendRequest();
    void getVacancies(); // TODO return vector of vacancies
private:
    QNetworkAccessManager networkManager;
    QNetworkRequest request;

private:
    const QString linkInit = "https://rs.linkedin.com/jobs/c%2B%2B-developer-jobs?countryRedirected=1&position=1&pageNum=0";
    const QString linkExpand = "https://rs.linkedin.com/jobs-guest/jobs/api/seeMoreJobPostings/c%2B%2B-developer-jobs?countryRedirected=1&position=1&pageNum=0&start=";
    QNetworkReply* reply;

public slots:
    void parseHTML(); // TODO temporrary
};