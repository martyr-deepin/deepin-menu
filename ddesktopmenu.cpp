#include <QPainter>
#include <QPainterPath>
#include <QBrush>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

#include "dmenubase.h"
#include "ddesktopmenu.h"
#include "dmenucontent.h"

DDesktopMenu::DDesktopMenu(DDesktopMenu *parent):
    DMenuBase(parent)
{
    this->setShadowMargins(QMargins(10, 10, 10, 10));
    this->setContentsMargins(this->shadowMargins());
    this->setMenuContentMargins(QMargins(20, 5, 15, 5));
    this->setItemLeftSpacing(10);
    this->setItemCenterSpacing(20);
    this->setItemRightSpacing(20);

    QSharedPointer<DMenuContent> ptr(new DMenuContent(this));
    this->setMenuContent(ptr);
}

// override methods
void DDesktopMenu::paintEvent(QPaintEvent *)
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
        this->setItemShortcutColor(QColor("#636363"));
        this->setCheckmarkIcon(":/images/check_light_normal.png");
        this->setSubMenuIndicatorIcon(":/images/arrow-light.png");
        break;
    case HoverState:
        this->setItemBackgroundColor(QColor("#535353"));
        this->setItemTextColor(Qt::white);
        this->setItemShortcutColor(QColor("#636363"));
        this->setCheckmarkIcon(":/images/check_light_hover.png");
        this->setSubMenuIndicatorIcon(":/images/arrow-light-hover.png");
        break;
    case InactiveState:
        this->setItemBackgroundColor(Qt::transparent);
        this->setItemTextColor(QColor("#b4b4b4"));
        this->setItemShortcutColor(QColor("#636363"));
        this->setCheckmarkIcon(":/images/check_light_inactive.png");
        this->setSubMenuIndicatorIcon(":/images/arrow-light.png");
        break;
    }
}

void DDesktopMenu::setPosition(int x, int y)
{
    this->move(x - this->shadowMargins().left(), y - this->shadowMargins().top());
}

void DDesktopMenu::showSubMenu(int x, int y, QJsonObject subMenuJsonObject)
{
    QJsonArray items = subMenuJsonObject["items"].toArray();
    if (items.count() != 0) {
        if (!_subMenu) {
            _subMenu = new DDesktopMenu(this);
        }
        _subMenu->setContent(items);
        _subMenu->setPosition(x, y);
        _subMenu->show();
    } else if (_subMenu && _subMenu->isVisible()) {
        _subMenu->hide();
    }
}
