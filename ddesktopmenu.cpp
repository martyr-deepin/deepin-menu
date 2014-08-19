#include <QPainter>
#include <QPainterPath>
#include <QBrush>
#include <QHBoxLayout>

#include "dmenubase.h"
#include "ddesktopmenu.h"
#include "dmenucontent.h"

DDesktopMenu::DDesktopMenu():
    DMenuBase()
{
    this->setShadowMargins(QMargins(10, 10, 10, 10));
    this->setContentsMargins(this->shadowMargins());
    this->setMenuContentMargins(QMargins(20, 5, 10, 5));
    this->setItemLeftSpacing(10);
    this->setItemCenterSpacing(10);
    this->setItemRightSpacing(10);

    QSharedPointer<DMenuContent> ptr(new DMenuContent(this));
    this->setMenuContent(ptr);
}

// override methods
void DDesktopMenu::paintEvent(QPaintEvent * event)
{
    QRect rect = this->rect().marginsRemoved(shadowMargins());

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath border;
    border.addRoundedRect(rect, radius(), radius());

    painter.fillPath(border, QBrush(Qt::white));
}

void DDesktopMenu::setItemState(ItemState state)
{
    switch (state) {
    case NormalState:
        this->setItemBackgroundColor(Qt::transparent);
        this->setItemTextColor(Qt::black);
        this->setItemShortcutColor(Qt::black);
        this->setCheckmarkIcon(":/images/check_light_normal.png");
        this->setSubMenuIndicatorIcon(":/images/arrow-light.png");
        break;
    case HoverState:
        this->setItemBackgroundColor(Qt::gray);
        this->setItemTextColor(Qt::white);
        this->setItemShortcutColor(Qt::white);
        this->setCheckmarkIcon(":/images/check_light_hover.png");
        this->setSubMenuIndicatorIcon(":/images/arrow-light-hover.png");
        break;
    case InactiveState:
        this->setItemBackgroundColor(Qt::transparent);
        this->setItemTextColor(Qt::gray);
        this->setItemShortcutColor(Qt::gray);
        this->setCheckmarkIcon(":/images/check_light_inactive.png");
        this->setSubMenuIndicatorIcon(":/images/arrow-light.png");
        break;
    }
}

void DDesktopMenu::setPosition(int x, int y)
{
    this->move(x - this->shadowMargins().left(), y - this->shadowMargins().top());
}
