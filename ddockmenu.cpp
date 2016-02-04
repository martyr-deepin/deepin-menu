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
#include <QRect>
#include <QPen>
#include <QBrush>
#include <QHBoxLayout>
#include <QJsonObject>
#include <QDebug>

#include "ddockmenu.h"
#include "dmenubase.h"
#include "dmenucontent.h"
#include "utils.h"

DDockMenu::DDockMenu(DDockMenu *parent):
    DMenuBase(parent),
    m_cornerX(0)
{
    this->setShadowMargins(QMargins(10, 10, 10, 10));
    this->setContentsMargins(QMargins(this->shadowMargins().left(),
                                      this->shadowMargins().top(),
                                      this->shadowMargins().right(),
                                      this->shadowMargins().bottom() + CORNER_HEIGHT));
    this->setMenuContentMargins(QMargins(5, 5, 5, 5));
    this->setItemLeftSpacing(10);
    this->setItemCenterSpacing(10);
    this->setItemRightSpacing(10);

    this->_normalStyle = ItemStyle{Qt::transparent,
            Qt::white,
            QColor("#646464"),
            ":/images/check_dark_normal.png",
            ":/images/arrow-dark.png"};
    this->_hoverStyle = ItemStyle{Qt::transparent,
            QColor("#00a4e2"),
            QColor("#646464"),
            ":/images/check_dark_hover.png",
            ":/images/arrow-dark.png"};
    this->_inactiveStyle = ItemStyle{Qt::transparent,
            QColor("#646464"),
            QColor("#646464"),
            ":/images/check_dark_inactive.png",
            ":/images/arrow-dark.png"};

    QSharedPointer<DMenuContent> ptr(new DMenuContent(this));
    this->setMenuContent(ptr);
}

// override methods
void DDockMenu::paintEvent(QPaintEvent *)
{
    QRect rect = this->rect().marginsRemoved(shadowMargins());

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int cornerX = m_cornerX ? m_cornerX : rect.x() + rect.width() / 2;
    QPoint topLeft(rect.x(), rect.y());
    QPoint topRight(rect.x() + rect.width(), rect.y());
    QPoint bottomRight(rect.x() + rect.width(), rect.y() + rect.height() - CORNER_HEIGHT);
    QPoint bottomLeft(rect.x(), rect.y() + rect.height() - CORNER_HEIGHT);
    QPoint cornerPoint(cornerX, rect.y() + rect.height());
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

void DDockMenu::setPosition(int x, int y)
{
    QPoint point(x - this->width() / 2,
                 y - this->height() + this->shadowMargins().bottom() / 2);
    QRect currentMonitorRect = Utils::currentMonitorRect(x, y);
    int deltaToMonitorLSide = point.x() -
            currentMonitorRect.x();

    int deltaToMonitorRSide = currentMonitorRect.x()
            + currentMonitorRect.width()
            - x - this->width() / 2;

    if (deltaToMonitorLSide < 0) {
        point.setX(point.x() - deltaToMonitorLSide);
        this->moveCornerX(deltaToMonitorLSide);
        this->repaint();
    }
    if (deltaToMonitorRSide < 0) {
        point.setX(point.x() + deltaToMonitorRSide);
        this->moveCornerX(-deltaToMonitorRSide);
        this->repaint();
    }

    this->move(point);
}

void DDockMenu::showSubMenu(int, int, QJsonObject)
{
}

// move the corner point horizontally from the central deltaX pixels.
void DDockMenu::moveCornerX(int deltaX)
{
    m_cornerX = this->width() / 2 + deltaX;
}
