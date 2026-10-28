#include <gtest/gtest.h>

#include <optional>

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QSet>
#include <QSignalSpy>
#include <QStringList>
#include <QTemporaryDir>
#include <QUrl>

#include "Observable.hpp"
#include "Utils.hpp"

TEST(ObservableTest, EmitsWhenTheValueChangesAndAssignmentsUseSetValue)
{
    Observable<int> value{4};
    Observable<int> source{8};
    QSignalSpy valueChangedSpy(&value, &AbstractObservable::valueChanged);

    EXPECT_EQ(value(), 4);
    EXPECT_EQ(value.getValue(), 4);

    value.setValue(4);
    EXPECT_EQ(valueChangedSpy.count(), 0);

    value.setValue(5);
    EXPECT_EQ(value(), 5);
    EXPECT_EQ(valueChangedSpy.count(), 1);

    value = 6;
    EXPECT_EQ(value.getValue(), 6);
    EXPECT_EQ(valueChangedSpy.count(), 2);

    value = source;
    EXPECT_EQ(value(), 8);
    EXPECT_EQ(valueChangedSpy.count(), 3);
}

TEST(UtilsTest, RemoveSpacesCollapsesInlineWhitespaceAndRemovesBlankLines)
{
    EXPECT_EQ(removeSpaces("first\t   line\n \t\nsecond  line"), QString("first line\nsecond line"));
}

TEST(UtilsTest, ExtractTrimsAndDecodesTheFirstCapture)
{
    EXPECT_EQ(extract("<span>  R&amp;D  </span>", R"(<span>(.*?)</span>)"), QString("R&D"));
    EXPECT_TRUE(extract("no match", R"(<span>(.*?)</span>)").isEmpty());
}

TEST(UtilsTest, ExtractAllTrimsAndDecodesEveryCapture)
{
    const QString html = "<li> First &amp; best </li><li> Second </li>";

    EXPECT_EQ(extractAll(html, R"(<li>(.*?)</li>)"), (QStringList{"First & best", "Second"}));
}

TEST(UtilsTest, DecodeHtmlReplacesSupportedEntities)
{
    EXPECT_EQ(decodeHtml("&amp;&quot;&#39;&lt;tag&gt;&nbsp;text &copy; &#x1F600;"), QString("&\"'<tag> text © 😀"));
}

TEST(UtilsTest, DecodeHtmlPreservesMarkupWhileDecodingEntities)
{
    EXPECT_EQ(decodeHtml("<strong>R&amp;D</strong>"), QString("<strong>R&D</strong>"));
}

TEST(UtilsTest, DecodeHtmlDecodesNestedEntities)
{
    EXPECT_EQ(decodeHtml("&amp;amp;"), QString("&"));
    EXPECT_EQ(decodeHtml("&amp;amp;amp;"), QString("&"));
    EXPECT_EQ(decodeHtml("<strong>&amp;amp;</strong>"), QString("<strong>&</strong>"));
}

TEST(UtilsTest, PrintWritesValuesAndSkipsEmptyOptionals)
{
    QString output;
    {
        QDebug debug(&output);
        print(debug, "count", 5);
        print(debug, "optional", std::optional<int>{7});
        print(debug, "empty", std::optional<int>{});
    }

    EXPECT_TRUE(output.contains("count"));
    EXPECT_TRUE(output.contains("5"));
    EXPECT_TRUE(output.contains("optional"));
    EXPECT_TRUE(output.contains("7"));
    EXPECT_FALSE(output.contains("empty"));
}

TEST(UtilsTest, UrlSetAndStringListRoundTrip)
{
    const QSet<QUrl> expected{
        QUrl("https://example.com/one"),
        QUrl("https://example.com/two")
    };
    const QStringList strings{
        "https://example.com/one",
        "https://example.com/two",
        "https://example.com/one"
    };

    EXPECT_EQ(toUrlSet(strings), expected);
    EXPECT_EQ(toUrlSet(toStringList(expected)), expected);
}

TEST(UtilsTest, SaveToFileWritesUtf8ContentIntoTheRequestedFolder)
{
    QTemporaryDir temporaryDirectory;
    ASSERT_TRUE(temporaryDirectory.isValid());

    const QString originalDirectory = QDir::currentPath();
    ASSERT_TRUE(QDir::setCurrent(temporaryDirectory.path()));

    const QString content = QString::fromUtf8("<title>R\xc3\xa9sum\xc3\xa9</title>");
    saveToFile(content, "page.html", "pages");

    const bool restoredDirectory = QDir::setCurrent(originalDirectory);
    ASSERT_TRUE(restoredDirectory);

    QFile file(temporaryDirectory.filePath("pages/page.html"));
    ASSERT_TRUE(file.open(QFile::ReadOnly));
    EXPECT_EQ(file.readAll(), QByteArray("<title>R\xc3\xa9sum\xc3\xa9</title>"));
}
