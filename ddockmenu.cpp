#include <QPainter>
#include <QPainterPath>
#include <QRect>
#include <QPen>
#include <QBrush>
#include <QHBoxLayout>
#include "ddockmenu.h"
#include "dmenubase.h"
#include "dmenucontent.h"

DDockMenu::DDockMenu():
    DMenuBase()
{
    this->setShadowMargins(QMargins(10, 10, 10, 10));
    this->setContentsMargins(this->shadowMargins());
    this->setItemLeftSpacing(10);
    this->setItemCenterSpacing(10);
    this->setItemRightSpacing(10);

    QMargins menuContentMargins = QMargins(10, 10, 10, 10);
    DMenuContent *content = new DMenuContent(this);
    content->setContentsMargins(menuContentMargins);
    this->resize(content->contentWidth()
                 + this->shadowMargins().left()
                 + this->shadowMargins().right()
                 + menuContentMargins.left()
                 + menuContentMargins.right(),
                 content->contentHeight()
                 + this->shadowMargins().top()
                 + this->shadowMargins().bottom()
                 + this->contentsMargins().top()
                 + this->contentsMargins().bottom());

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(QMargins(0, menuContentMargins.top(), 0, menuContentMargins.bottom()));
    layout->addWidget(content);

    this->setLayout(layout);
}

// override methods
void DDockMenu::paintEvent(QPaintEvent *)
{
    QRect rect = this->rect().marginsRemoved(shadowMargins());

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPoint topLeft(rect.x(), rect.y());
    QPoint topRight(rect.x() + rect.width(), rect.y());
    QPoint bottomRight(rect.x() + rect.width(), rect.y() + rect.height() - CORNER_HEIGHT);
    QPoint bottomLeft(rect.x(), rect.y() + rect.height() - CORNER_HEIGHT);
    QPoint cornerPoint(rect.x() + rect.width() / 2, rect.y() + rect.height());
    int radius = this->radius();

    QPainterPath border;
    border.moveTo(topLeft.x() + radius, topLeft.y());
    border.lineTo(topRight.x() - radius, topRight.y());
    border.arcTo(topRight.x() - 2 * radius, topRight.y(), 2 * radius, 2 * radius, 90, -90);
    border.lineTo(bottomRight.x(), bottomRight.y() - radius);
    border.arcTo(bottomRight.x() - 2 * radius, bottomRight.y() - 2 * radius, 2 * radius, 2 * radius, 0, -90);
    border.lineTo(cornerPoint.x() + CORNER_WIDTH / 2, cornerPoint.y() - CORNER_HEIGHT);
    border.lineTo(cornerPoint);
    border.lineTo(cornerPoint.x() - CORNER_WIDTH / 2, cornerPoint.y() - CORNER_HEIGHT);
    border.lineTo(bottomLeft.x() + radius, bottomLeft.y());
    border.arcTo(bottomLeft.x(), bottomLeft.y() - 2 * radius, 2 * radius, 2 * radius, -90, -90);
    border.lineTo(topLeft.x(), topLeft.y() + radius);
    border.arcTo(topLeft.x(), topLeft.y(), 2 * radius, 2 * radius, 180, -90);

    painter.strokePath(border, QPen(Qt::white));
    painter.fillPath(border, QBrush(Qt::black));
}

void DDockMenu::setItemState(ItemState state)
{
    switch (state) {
    case NormalState:
        this->setItemBackgroundColor(Qt::transparent);
        this->setItemTextColor(Qt::black);
        this->setItemShortcutColor(Qt::black);
        break;
    case HoverState:
        this->setItemBackgroundColor(Qt::gray);
        this->setItemTextColor(Qt::white);
        this->setItemShortcutColor(Qt::white);
        break;
    case InactiveState:
        this->setItemBackgroundColor(Qt::transparent);
        this->setItemTextColor(Qt::gray);
        this->setItemShortcutColor(Qt::gray);
        break;
    default: break;
    }
}

void DDockMenu::setPosition(int x, int y)
{
    this->move(x - this->width() / 2, y - this->height() + this->shadowMargins().bottom());
}
