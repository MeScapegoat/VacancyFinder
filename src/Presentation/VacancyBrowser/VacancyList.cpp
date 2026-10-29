#include "VacancyList.h"

#include <QAbstractItemView>
#include <QColor>
#include <QFont>
#include <QFontMetrics>
#include <QFrame>
#include <QModelIndex>
#include <QPainter>
#include <QPalette>
#include <QPen>
#include <QRect>
#include <QSize>
#include <QString>
#include <QStyle>
#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>
#include <QWidget>

#include "VacancyListModel.h"

namespace
{
class VacancyCardDelegate final : public QStyledItemDelegate
{
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);

        const QRect cardRect = option.rect.adjusted(5, 5, -5, -5);
        const bool isSelected = option.state.testFlag(QStyle::State_Selected);
        const bool isHovered = option.state.testFlag(QStyle::State_MouseOver);
        const QPalette& palette = option.palette;
        const bool isDarkTheme = palette.color(QPalette::Base).lightness() < 128;
        const QColor backgroundColor = palette.color(isSelected ? QPalette::Highlight : QPalette::Base);
        const QColor borderColor = palette.color(isSelected ? QPalette::Highlight : isDarkTheme ? QPalette::Midlight : QPalette::Mid);

        painter->setPen(QPen(borderColor, isSelected ? 2.5 : isHovered ? 2 : 1.5));
        painter->setBrush(backgroundColor);
        painter->drawRoundedRect(cardRect, 12, 12);

        const QString status = index.data(VacancyListModel::StatusLabelRole).toString();
        const QColor vacancyTextColor = palette.color(isSelected ? QPalette::HighlightedText : QPalette::Text);
        const QFont baseFont = option.font;
        const QFontMetrics baseMetrics(baseFont);
        const int badgeWidth = baseMetrics.horizontalAdvance(status) + 22;
        const QRect badgeRect(cardRect.right() - badgeWidth - 12, cardRect.top() + 12, badgeWidth, 22);

        QColor badgeBackground = palette.color(isDarkTheme ? QPalette::Midlight : QPalette::Mid);
        QColor badgeForeground = vacancyTextColor;
        if (status == "Interested")
        {
            badgeBackground = palette.color(QPalette::Highlight);
            badgeForeground = palette.color(QPalette::HighlightedText);
        }
        if (isSelected)
        {
            badgeBackground = badgeBackground.darker(125);
        }
        painter->setPen(Qt::NoPen);
        painter->setBrush(badgeBackground);
        painter->drawRoundedRect(badgeRect, 11, 11);

        QFont badgeFont = baseFont;
        badgeFont.setPointSize(8);
        badgeFont.setWeight(QFont::DemiBold);
        painter->setFont(badgeFont);
        painter->setPen(badgeForeground);
        painter->drawText(badgeRect, Qt::AlignCenter, status);

        const int left = cardRect.left() + 15;
        const int textWidth = badgeRect.left() - left - 10;
        const QString title = index.data(VacancyListModel::TitleRole).toString();
        const QString company = index.data(VacancyListModel::CompanyRole).toString();
        const QString location = index.data(VacancyListModel::LocationRole).toString();
        const QString age = index.data(VacancyListModel::AgeRole).toString();

        QFont titleFont = baseFont;
        titleFont.setPointSize(11);
        titleFont.setWeight(QFont::DemiBold);
        painter->setFont(titleFont);
        painter->setPen(vacancyTextColor);
        painter->drawText(QRect(left, cardRect.top() + 10, textWidth, 25), Qt::AlignVCenter | Qt::AlignLeft,
                          QFontMetrics(titleFont).elidedText(title, Qt::ElideRight, textWidth));

        QFont companyFont = baseFont;
        companyFont.setPointSize(9);
        painter->setFont(companyFont);
        painter->setPen(vacancyTextColor);
        painter->drawText(QRect(left, cardRect.top() + 38, cardRect.width() - 30, 19), Qt::AlignVCenter | Qt::AlignLeft,
                          QFontMetrics(companyFont).elidedText(company, Qt::ElideRight, cardRect.width() - 30));

        QFont detailFont = baseFont;
        detailFont.setPointSize(9);
        QFont ageFont = detailFont;
        ageFont.setWeight(QFont::DemiBold);
        painter->setFont(detailFont);
        painter->setPen(vacancyTextColor);
        const QRect detailRect(left, cardRect.top() + 63, cardRect.width() - 30, 18);
        const int ageWidth = QFontMetrics(ageFont).horizontalAdvance(age) + 8;
        const QRect locationRect = detailRect.adjusted(0, 0, -ageWidth, 0);
        const QRect ageRect(detailRect.right() - ageWidth, detailRect.top(), ageWidth, detailRect.height());
        painter->drawText(locationRect, Qt::AlignVCenter | Qt::AlignLeft,
                          QFontMetrics(detailFont).elidedText(location, Qt::ElideRight, locationRect.width()));
        painter->setFont(ageFont);
        painter->setPen(vacancyTextColor);
        painter->drawText(ageRect, Qt::AlignVCenter | Qt::AlignRight, age);

        painter->restore();
    }

    QSize sizeHint(const QStyleOptionViewItem&, const QModelIndex&) const override
    {
        return QSize(260, 96);
    }
};
}

VacancyList::VacancyList(QWidget* parent) :
    QListView(parent)
{
    setObjectName("vacancyList");
    setFrameShape(QFrame::StyledPanel);
    setBackgroundRole(QPalette::Window);
    setAutoFillBackground(true);
    viewport()->setBackgroundRole(QPalette::Window);
    viewport()->setAutoFillBackground(true);
    setItemDelegate(new VacancyCardDelegate(this));
    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setUniformItemSizes(true);
    setWrapping(false);
    setSpacing(8);
    setMouseTracking(true);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}
