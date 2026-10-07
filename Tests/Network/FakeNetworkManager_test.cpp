#include "FakeNetworkManager.h"
#include "FakeNetworkReply.h"

#include <gtest/gtest.h>

#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QSignalSpy>
#include <QUrl>

class FakeNetworkManagerTest : public ::testing::Test
{
protected:
    QNetworkRequest request{
        QUrl("url")
    };
};

TEST_F(FakeNetworkManagerTest, CookieJarIsNotNullByDefault)
{
    FakeNetworkManager manager;

    EXPECT_NE(manager.cookieJar(), nullptr);
}

TEST_F(FakeNetworkManagerTest, SetCookieJar)
{
    FakeNetworkManager manager;

    auto* cookieJar = new QNetworkCookieJar;

    manager.setCookieJar(cookieJar);

    EXPECT_EQ(manager.cookieJar(), cookieJar);
}

TEST_F(FakeNetworkManagerTest, SettingSameCookieJarDoesNotDeleteIt)
{
    FakeNetworkManager manager;

    auto* cookieJar = new QNetworkCookieJar;

    manager.setCookieJar(cookieJar);
    manager.setCookieJar(cookieJar);

    EXPECT_EQ(manager.cookieJar(), cookieJar);
}

TEST_F(FakeNetworkManagerTest, GetReturnsCorrectReply)
{
    FakeNetworkManager manager;
    const QByteArray expected = "Hello World";

    manager.respondWith(
        200,
        expected,
        QNetworkReply::InternalServerError
    );

    QNetworkReply* reply = manager.get(request);

    ASSERT_NE(reply, nullptr);

    EXPECT_EQ(reply->request().url(), request.url());
    EXPECT_EQ(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 200);
    EXPECT_EQ(reply->readAll(), expected);
    EXPECT_EQ(reply->error(), QNetworkReply::InternalServerError);

    delete reply;
}

TEST_F(FakeNetworkManagerTest, FinishEmitsFinished)
{
    qRegisterMetaType<QNetworkReply*>();
    FakeNetworkManager manager;

    manager.respondWith(200, QByteArray("Hello"), QNetworkReply::NoError);

    QNetworkReply* reply = manager.get(request);

    ASSERT_NE(reply, nullptr);

    QSignalSpy spy(&manager, &FakeNetworkManager::finished);

    manager.finish();

    ASSERT_EQ(spy.count(), 1);

    const QList<QVariant> arguments = spy.takeFirst();

    ASSERT_EQ(arguments.size(), 1);

    EXPECT_EQ(arguments.at(0).value<QNetworkReply*>(), reply);

    delete reply;
}