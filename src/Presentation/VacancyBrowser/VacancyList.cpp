#include "VacancyList.h"

#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QColor>
#include <QFont>
#include <QFontMetrics>
#include <QFrame>
#include <QHBoxLayout>
#include <QLabel>
#include <QListView>
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
#include <QVBoxLayout>
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
    QWidget(parent),
    m_listView(*new QListView(this)),
    m_visibleVacancyCount(*new QLabel(this))
{
    setObjectName("vacancyListPanel");

    QVBoxLayout& mainLayout = *new QVBoxLayout(this);
    mainLayout.setContentsMargins(0, 0, 0, 0);
    mainLayout.setSpacing(8);

    QLabel& heading = *new QLabel("Your vacancies", this);
    heading.setObjectName("vacancyListHeading");
    QFont headingFont = heading.font();
    headingFont.setPointSize(12);
    headingFont.setWeight(QFont::DemiBold);
    heading.setFont(headingFont);

    QHBoxLayout& headingLayout = *new QHBoxLayout;
    headingLayout.setContentsMargins(0, 0, 0, 0);
    headingLayout.addWidget(&heading);
    headingLayout.addStretch();
    m_visibleVacancyCount.setObjectName("visibleVacancyCount");
    m_visibleVacancyCount.setFont(headingFont);
    m_visibleVacancyCount.setText(tr("Visible vacancies: 0"));
    headingLayout.addWidget(&m_visibleVacancyCount);
    mainLayout.addLayout(&headingLayout);

    QLabel& caption = *new QLabel("Select a role to review its details", this);
    caption.setObjectName("vacancyListCaption");
    QFont captionFont = caption.font();
    captionFont.setPointSize(8);
    caption.setFont(captionFont);
    caption.setForegroundRole(QPalette::WindowText);
    mainLayout.addWidget(&caption);

    m_listView.setObjectName("vacancyList");
    m_listView.setFrameShape(QFrame::StyledPanel);
    m_listView.setBackgroundRole(QPalette::Window);
    m_listView.setAutoFillBackground(true);
    m_listView.viewport()->setBackgroundRole(QPalette::Window);
    m_listView.viewport()->setAutoFillBackground(true);
    m_listView.setItemDelegate(new VacancyCardDelegate(&m_listView));
    m_listView.setSelectionMode(QAbstractItemView::SingleSelection);
    m_listView.setSelectionBehavior(QAbstractItemView::SelectRows);
    m_listView.setUniformItemSizes(true);
    m_listView.setWrapping(false);
    m_listView.setSpacing(8);
    m_listView.setMouseTracking(true);
    m_listView.setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_listView.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mainLayout.addWidget(&m_listView, 1);
}

QListView& VacancyList::listView()
{
    return m_listView;
}

const QListView& VacancyList::listView() const
{
    return m_listView;
}

void VacancyList::setModel(QAbstractItemModel* model)
{
    m_listView.setModel(model);
    if (model)
    {
        QObject::connect(model, &QAbstractItemModel::modelReset, this, [this]
        {
            updateVisibleVacancyCount();
        });
    }

    updateVisibleVacancyCount();
}

void VacancyList::updateVisibleVacancyCount()
{
    const int count = m_listView.model() ? m_listView.model()->rowCount() : 0;
    m_visibleVacancyCount.setText(tr("Visible vacancies: %1").arg(count));
}
