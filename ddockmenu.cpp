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
    m_cornerX(0),
    m_cornerY(0)
{
    this->setShadowMargins(QMargins(20, 20, 20, 20));
    this->setMenuContentMargins(QMargins(5, 5, 5, 5));
    this->setItemLeftSpacing(10);
    this->setItemCenterSpacing(10);
    this->setItemRightSpacing(10);

    this->setDirection(Bottom);

    this->_normalStyle = ItemStyle{Qt::transparent,
            Qt::white,
            QColor("#646464"),
            ":/images/check_dark_normal.png",
            ":/images/arrow-dark.png"};
    this->_hoverStyle = ItemStyle{QColor("#2ca7f8"),
            Qt::white,
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
    QColor bdcolor = QColor::fromRgb(255, 255, 255, 255 * 0.1);
    QColor bgcolor = QColor::fromRgb(18, 18, 18, 255 * 0.9);
    QRect rect = this->rect().marginsRemoved(shadowMargins());

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (m_direction == Bottom) {
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

        painter.strokePath(border, bdcolor);
        painter.fillPath(border, bgcolor);

    } else if(m_direction == Top) {
        int cornerX = m_cornerX ? m_cornerX : rect.x() + rect.width() / 2;
        QPoint topLeft(rect.x(), rect.y() + CORNER_HEIGHT);
        QPoint topRight(rect.x() + rect.width(), rect.y() + CORNER_HEIGHT);
        QPoint bottomRight(rect.x() + rect.width(), rect.y() + rect.height());
        QPoint bottomLeft(rect.x(), rect.y() + rect.height());
        QPoint cornerPoint(cornerX, rect.y());
        int radius = this->radius();

        QPainterPath border;
        border.moveTo(topLeft.x() + radius, topLeft.y());
        border.lineTo(cornerPoint.x() - CORNER_WIDTH / 2, cornerPoint.y() + CORNER_HEIGHT);
        border.lineTo(cornerPoint);
        border.lineTo(cornerPoint.x() + CORNER_WIDTH / 2, cornerPoint.y() + CORNER_HEIGHT);
        border.lineTo(topRight.x() - radius, topRight.y());
        border.arcTo(topRight.x() - 2 * radius, topRight.y(), 2 * radius, 2 * radius, 90, -90);
        border.lineTo(bottomRight.x(), bottomRight.y() - radius);
        border.arcTo(bottomRight.x() - 2 * radius, bottomRight.y() - 2 * radius, 2 * radius, 2 * radius, 0, -90);
        border.lineTo(bottomLeft.x() + radius, bottomLeft.y());
        border.arcTo(bottomLeft.x(), bottomLeft.y() - 2 * radius, 2 * radius, 2 * radius, -90, -90);
        border.lineTo(topLeft.x(), topLeft.y() + radius);
        border.arcTo(topLeft.x(), topLeft.y(), 2 * radius, 2 * radius, 180, -90);

        painter.strokePath(border, bdcolor);
        painter.fillPath(border, bgcolor);
    } else if(m_direction == Left) {
        int cornerY = m_cornerY ? m_cornerY : rect.y() + rect.height() / 2;
        QPoint topLeft(rect.x() + CORNER_HEIGHT, rect.y());
        QPoint topRight(rect.x() + rect.width(), rect.y());
        QPoint bottomRight(rect.x() + rect.width(), rect.y() + rect.height());
        QPoint bottomLeft(rect.x() + CORNER_HEIGHT, rect.y() + rect.height());
        QPoint cornerPoint(rect.x(), cornerY);
        int radius = this->radius();

        QPainterPath border;
        border.moveTo(topLeft.x() + radius, topLeft.y());     
        border.lineTo(topRight.x() - radius, topRight.y());
        border.arcTo(topRight.x() - 2 * radius, topRight.y(), 2 * radius, 2 * radius, 90, -90);
        border.lineTo(bottomRight.x(), bottomRight.y() - radius);
        border.arcTo(bottomRight.x() - 2 * radius, bottomRight.y() - 2 * radius, 2 * radius, 2 * radius, 0, -90);
        border.lineTo(bottomLeft.x() + radius, bottomLeft.y());
        border.arcTo(bottomLeft.x(), bottomLeft.y() - 2 * radius, 2 * radius, 2 * radius, -90, -90);
        border.lineTo(cornerPoint.x() + CORNER_HEIGHT, cornerPoint.y() + CORNER_WIDTH / 2);
        border.lineTo(cornerPoint);
        border.lineTo(cornerPoint.x() + CORNER_HEIGHT, cornerPoint.y() - CORNER_WIDTH / 2);
        border.lineTo(topLeft.x(), topLeft.y() + radius);
        border.arcTo(topLeft.x(), topLeft.y(), 2 * radius, 2 * radius, 180, -90);

        painter.strokePath(border, bdcolor);
        painter.fillPath(border, bgcolor);
    } else {
        int cornerY = m_cornerY ? m_cornerY : rect.y() + rect.height() / 2;
        QPoint topLeft(rect.x(), rect.y());
        QPoint topRight(rect.x() + rect.width() - CORNER_HEIGHT, rect.y());
        QPoint bottomRight(rect.x() + rect.width() - CORNER_HEIGHT, rect.y() + rect.height());
        QPoint bottomLeft(rect.x(), rect.y() + rect.height());
        QPoint cornerPoint(rect.x() + rect.width(), cornerY);
        int radius = this->radius();

        QPainterPath border;
        border.moveTo(topLeft.x() + radius, topLeft.y());
        border.lineTo(topRight.x() - radius, topRight.y());
        border.arcTo(topRight.x() - 2 * radius, topRight.y(), 2 * radius, 2 * radius, 90, -90);
        border.lineTo(cornerPoint.x() - CORNER_HEIGHT, cornerPoint.y() - CORNER_WIDTH / 2);
        border.lineTo(cornerPoint);
        border.lineTo(cornerPoint.x() - CORNER_HEIGHT, cornerPoint.y() + CORNER_WIDTH / 2);
        border.lineTo(bottomRight.x(), bottomRight.y() - radius);
        border.arcTo(bottomRight.x() - 2 * radius, bottomRight.y() - 2 * radius, 2 * radius, 2 * radius, 0, -90);
        border.lineTo(bottomLeft.x() + radius, bottomLeft.y());
        border.arcTo(bottomLeft.x(), bottomLeft.y() - 2 * radius, 2 * radius, 2 * radius, -90, -90);
        border.lineTo(topLeft.x(), topLeft.y() + radius);
        border.arcTo(topLeft.x(), topLeft.y(), 2 * radius, 2 * radius, 180, -90);

        painter.strokePath(border, bdcolor);
        painter.fillPath(border, bgcolor);
    }
}

DDockMenu::Direction DDockMenu::direction() const
{
    return m_direction;
}

void DDockMenu::setDirection(const Direction &direction)
{
    if (m_direction != direction) {
        m_direction = direction;
        setContentsMargins(QMargins(shadowMargins().left() + (m_direction == Left ? CORNER_HEIGHT : 0),
                                    shadowMargins().top() + (m_direction == Top ? CORNER_HEIGHT : 0),
                                    shadowMargins().right() + (m_direction == Right ? CORNER_HEIGHT : 0),
                                    shadowMargins().bottom()  + (m_direction == Bottom ? CORNER_HEIGHT : 0)));
    }
}

void DDockMenu::setPosition(int x, int y)
{
    if (m_direction == Top) {
        QPoint point(x - this->width() / 2, y);
        QRect currentMonitorRect = Utils::currentMonitorRect(x, y);
        int deltaToMonitorLSide = point.x() -
                currentMonitorRect.x();

        int deltaToMonitorRSide = currentMonitorRect.x()
                + currentMonitorRect.width()
                - point.x();

        if (deltaToMonitorLSide < 0) {
            point.setX(point.x() - deltaToMonitorLSide);
            this->moveCornerX(deltaToMonitorLSide);
        }
        if (deltaToMonitorRSide < 0) {
            point.setX(point.x() + deltaToMonitorRSide);
            this->moveCornerX(-deltaToMonitorRSide);
        }

        this->move(point);
    } else if (m_direction == Bottom) {
        QPoint point(x - this->width() / 2,
                     y - this->height() + this->shadowMargins().bottom() / 2);
        QRect currentMonitorRect = Utils::currentMonitorRect(x, y);
        int deltaToMonitorLSide = point.x() -
                currentMonitorRect.x();

        int deltaToMonitorRSide = currentMonitorRect.x()
                + currentMonitorRect.width()
                - point.x();

        if (deltaToMonitorLSide < 0) {
            point.setX(point.x() - deltaToMonitorLSide);
            this->moveCornerX(deltaToMonitorLSide);
        }
        if (deltaToMonitorRSide < 0) {
            point.setX(point.x() + deltaToMonitorRSide);
            this->moveCornerX(-deltaToMonitorRSide);
        }

        this->move(point);
    } else if (m_direction == Left) {
        QPoint point(x, y - this->height() / 2);
        QRect currentMonitorRect = Utils::currentMonitorRect(x, y);
        int deltaToMonitorTSide = point.y() -
                currentMonitorRect.y();

        int deltaToMonitorBSide = currentMonitorRect.y()
                + currentMonitorRect.height()
                - point.y();

        if (deltaToMonitorTSide < 0) {
            point.setY(point.y() - deltaToMonitorTSide);
            this->moveCornerY(deltaToMonitorTSide);
        }
        if (deltaToMonitorBSide < 0) {
            point.setY(point.y() + deltaToMonitorBSide);
            this->moveCornerY(deltaToMonitorBSide);
        }

        this->move(point);
    } else {
        QPoint point(x - this->width(), y - this->height() / 2);
        QRect currentMonitorRect = Utils::currentMonitorRect(x, y);
        int deltaToMonitorTSide = point.y() -
                currentMonitorRect.y();

        int deltaToMonitorBSide = currentMonitorRect.y()
                + currentMonitorRect.height()
                - point.y();

        if (deltaToMonitorTSide < 0) {
            point.setY(point.y() - deltaToMonitorTSide);
            this->moveCornerY(deltaToMonitorTSide);
        }
        if (deltaToMonitorBSide < 0) {
            point.setY(point.y() + deltaToMonitorBSide);
            this->moveCornerY(deltaToMonitorBSide);
        }

        this->move(point);
    }
}

void DDockMenu::showSubMenu(int, int, QJsonObject)
{
}

// move the corner point horizontally from the central deltaX pixels.
void DDockMenu::moveCornerX(int deltaX)
{
    m_cornerX = this->width() / 2 + deltaX;
}

void DDockMenu::moveCornerY(int deltaY)
{
    m_cornerY = this->height() / 2 + deltaY;
}
