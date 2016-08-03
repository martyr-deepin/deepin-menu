/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QPainter>
#include <QPainterPath>
#include <QBrush>
#include <QJsonObject>
#include <QJsonArray>
#include <QtGlobal>
#include <QDebug>

#include "utils.h"
#include "dmenubase.h"
#include "ddesktopmenu.h"
#include "dmenucontent.h"

DDesktopMenu::DDesktopMenu(DDesktopMenu *parent):
    DMenuBase(parent)
{
    this->setShadowMargins(QMargins(20, 20, 20, 20));
    this->setContentsMargins(this->shadowMargins());
    this->setMenuContentMargins(QMargins(5, 5, 5, 5));
    this->setItemLeftSpacing(10);
    this->setItemCenterSpacing(20);
    this->setItemRightSpacing(20);

    this->_normalStyle = { Qt::transparent,
                           QColor("#303030"),
                           QColor("#636363"),
                           ":/images/check_light_normal.png",
                           ":/images/arrow-light.png" };
    this->_hoverStyle = { QColor("#2ca7f8"),
                          Qt::white,
                          QColor("#636363"),
                          ":/images/check_light_hover.png",
                          ":/images/arrow-light-hover.png" };
    this->_inactiveStyle = { Qt::transparent,
                             QColor("#b4b4b4"),
                             QColor("#636363"),
                             ":/images/check_light_inactive.png",
                             ":/images/arrow-light-inactive.png" };

    QSharedPointer<DMenuContent> ptr(new DMenuContent(this));
    this->setMenuContent(ptr);
}

// override methods
void DDesktopMenu::paintEvent(QPaintEvent *)
{
    QRect rect = this->rect().marginsRemoved(shadowMargins());

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    path.addRoundedRect(rect, radius(), radius());

    QPen pen = painter.pen();
    pen.setColor(QColor::fromRgbF(0, 0, 0, 0.15));
    pen.setWidth(1);
    painter.strokePath(path, pen);

    painter.fillPath(path, Qt::white);
}

void DDesktopMenu::setPosition(int x, int y)
{
    DMenuBase *rootMenu = this->getRootMenu();
    Q_ASSERT(rootMenu);

    QPoint point(x - this->shadowMargins().left(),
                 y - this->shadowMargins().top());
    QRect currentMonitorRect = (this == rootMenu) ?  Utils::currentMonitorRect(x, y)
                                                   : Utils::currentMonitorRect(rootMenu->x() + rootMenu->shadowMargins().left(),
                                                                               rootMenu->y() + rootMenu->shadowMargins().top());

    if (point.x()
            + this->width()
            > currentMonitorRect.x()
            + currentMonitorRect.width()) {
        if (this->parent()) {
            DMenuBase *parent = qobject_cast<DMenuBase *>(this->parent());
            Q_ASSERT(parent);

            point.setX(parent->x()
                       + parent->shadowMargins().left()
                       - this->width()
                       + this->shadowMargins().right());
        } else {
            point.setX(point.x()
                       - this->width()
                       + this->shadowMargins().left()
                       + this->shadowMargins().right());
        }
    }

    if (point.y()
            + this->height()
            > currentMonitorRect.y()
            + currentMonitorRect.height()) {
        if (this->parent()) {
            point.setY(currentMonitorRect.y()
                       + currentMonitorRect.height()
                       - this->height());
        } else {
            point.setY(point.y()
                       - this->height()
                       + this->shadowMargins().top()
                       + this->shadowMargins().bottom());
        }
    }

    this->move(point);
}

void DDesktopMenu::showSubMenu(int x, int y, QJsonObject subMenuJsonObject)
{
    qDebug() << "showSubMenu " << x << y << subMenuJsonObject;

    QJsonArray items = subMenuJsonObject["items"].toArray();
    if (items.count() != 0) {
        if (!_subMenu) {
            _subMenu = new DDesktopMenu(this);
        }
        _subMenu->setContent(items);
        _subMenu->setPosition(x, y);
        _subMenu->show();
        _subMenu->grabFocus();
        _subMenu->menuContent()->setCurrentIndex(0);
    } else if (_subMenu && _subMenu->isVisible()) {
        _subMenu->deleteLater();
        _subMenu = NULL;
    }
}
