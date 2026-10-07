#include "FakeNetworkReply.h"

#include <gtest/gtest.h>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QSignalSpy>
#include <QUrl>

class FakeNetworkReplyTest : public ::testing::Test
{
protected:
    QNetworkRequest request{
        QUrl("url")
    };
};

TEST_F(FakeNetworkReplyTest, SetsRequest)
{
    FakeNetworkReply reply(
        request,
        200,
        QByteArray("Hello")
    );

    EXPECT_EQ(reply.request().url(), request.url());
}

TEST_F(FakeNetworkReplyTest, SetsUrl)
{
    FakeNetworkReply reply(
        request,
        200,
        QByteArray("Hello")
    );

    EXPECT_EQ(reply.url(), request.url());
}

TEST_F(FakeNetworkReplyTest, SetsStatusCode)
{
    FakeNetworkReply reply(
        request,
        201,
        QByteArray("Created")
    );

    EXPECT_EQ(reply.attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 201);
}

TEST_F(FakeNetworkReplyTest, SetsNetworkError)
{
    FakeNetworkReply reply(
        request,
        200,
        QByteArray("Hello"),
        QNetworkReply::TimeoutError
    );

    EXPECT_EQ(reply.error(), QNetworkReply::TimeoutError);
}

TEST_F(FakeNetworkReplyTest, NoErrorByDefault)
{
    FakeNetworkReply reply(
        request,
        200,
        QByteArray("Hello")
    );

    EXPECT_EQ(reply.error(), QNetworkReply::NoError);
}

TEST_F(FakeNetworkReplyTest, ReadDataReturnsData)
{
    const QByteArray expected = "Hello World";

    FakeNetworkReply reply(
        request,
        200,
        expected
    );

    const QByteArray actual = reply.readAll();

    EXPECT_EQ(actual, expected);
}

TEST_F(FakeNetworkReplyTest, AbortSetsOperationCanceledError)
{
    FakeNetworkReply reply(
        request,
        200,
        QByteArray("Hello")
    );

    reply.abort();

    EXPECT_EQ(
        reply.error(),
        QNetworkReply::OperationCanceledError
    );
}

TEST_F(FakeNetworkReplyTest, AbortEmitsFinished)
{
    FakeNetworkReply reply(
        request,
        200,
        QByteArray("Hello")
    );

    QSignalSpy spy(&reply, &QNetworkReply::finished);

    reply.abort();

    ASSERT_EQ(spy.count(), 1);
}

TEST_F(FakeNetworkReplyTest, EmptyDataCanBeRead)
{
    FakeNetworkReply reply(
        request,
        200,
        QByteArray()
    );

    EXPECT_TRUE(reply.readAll().isEmpty());
}