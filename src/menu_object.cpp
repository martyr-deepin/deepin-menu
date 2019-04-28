/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     Hualet Wang <mr.asianwang@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QColor>
#include <QGuiApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtGlobal>
#include <QStyle>
#include <QDebug>
#include <QScreen>

#include "menu_object.h"
#include "ddesktopmenu.h"
#include "ddockmenu.h"

static DArrowRectangle::ArrowDirection DirectionFromString(QString direction) {
    if (direction == "top") {
        return DArrowRectangle::ArrowTop;
    } else if (direction == "left") {
        return DArrowRectangle::ArrowLeft;
    } else if (direction == "right") {
        return DArrowRectangle::ArrowRight;
    }

    return DArrowRectangle::ArrowBottom;
}

MenuObject::MenuObject():
    QObject(),
    m_dockMenu(nullptr),
    m_desktopMenu(nullptr)
{

}

MenuObject::~MenuObject()
{
    if (!m_dockMenu.isNull()) {
        delete m_dockMenu;
    }

    if (!m_desktopMenu.isNull()) {
        delete m_desktopMenu;
    }
}

void MenuObject::SetItemActivity(const QString &itemId, bool isActive)
{
    if (!m_dockMenu.isNull()) m_dockMenu->setItemActivity(itemId, isActive);
    if (!m_desktopMenu.isNull()) m_desktopMenu->setItemActivity(itemId, isActive);
}

void MenuObject::SetItemChecked(const QString &itemId, bool checked)
{
    if (!m_dockMenu.isNull()) m_dockMenu->setItemChecked(itemId, checked);
    if (!m_desktopMenu.isNull()) m_desktopMenu->setItemChecked(itemId, checked);
}

void MenuObject::SetItemText(const QString &itemId, const QString &text)
{
    if (!m_dockMenu.isNull()) m_dockMenu->setItemText(itemId, text);
    if (!m_desktopMenu.isNull()) m_desktopMenu->setItemText(itemId, text);
}

void MenuObject::ShowMenu(const QString &menuJsonContent)
{
    QByteArray bytes;
    bytes.append(menuJsonContent);
    QJsonDocument menuDocument = QJsonDocument::fromJson(bytes);
    QJsonObject jsonObj = menuDocument.object();

    int x = jsonObj["x"].toDouble();
    int y = jsonObj["y"].toDouble();
    QString direction = jsonObj["direction"].toString();

    if(jsonObj["isDockMenu"].toBool()) {
        m_dockMenu = new DDockMenu;
        connect(m_dockMenu, &DDockMenu::destroyed, this, &MenuObject::menuDismissedSlot, Qt::QueuedConnection);
        connect(m_dockMenu, &DDockMenu::itemClicked, this, &MenuObject::ItemInvoked);
    } else {
        m_desktopMenu = new DDesktopMenu;
        connect(m_desktopMenu, &DDesktopMenu::aboutToHide, this, &MenuObject::menuDismissedSlot, Qt::QueuedConnection);
        connect(m_desktopMenu, &DDesktopMenu::itemClicked, this, &MenuObject::ItemInvoked);
    }

    bytes.clear();
    bytes.append(jsonObj["menuJsonContent"].toString());
    QJsonDocument menuContent = QJsonDocument::fromJson(bytes);
    QJsonObject menuContentObj = menuContent.object();

    if (!m_dockMenu.isNull()) {
        m_dockMenu->setArrowDirection(DirectionFromString(direction));
        m_dockMenu->setItems(menuContentObj["items"].toArray());
        m_dockMenu->show(x, y);
    } else if (!m_desktopMenu.isNull()) {
        m_desktopMenu->setItems(menuContentObj["items"].toArray());

        // 在Qt 5.10.x上, 菜单 show 之前没有被polish, 导致 dstyle 中无法将菜单设置为"圆角+模糊"样式
        if (m_desktopMenu->style() && !m_desktopMenu->testAttribute(Qt::WA_WState_Polished)) {
            m_desktopMenu->style()->polish(m_desktopMenu);
        }

        bool isScaled = true;
        if (!jsonObj["isScaled"].isNull()) {
            isScaled = jsonObj["isScaled"].toBool();
        }

        m_desktopMenu->showMenu(QPoint(x, y), isScaled);
    }
}

void MenuObject::menuDismissedSlot()
{
    emit MenuUnregistered();

    if (!m_dockMenu.isNull()) {
        m_dockMenu->deleteLater();
        m_dockMenu = nullptr;
    }

    if (!m_desktopMenu.isNull()) {
        m_desktopMenu->deleteLater();
        m_desktopMenu = nullptr;
    }

    deleteLater();
}
