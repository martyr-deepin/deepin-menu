/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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

#include "ddesktopmenu.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QKeyEvent>
#include <QDBusPendingCall>
#include <QTimer>

DDesktopMenu::DDesktopMenu() :
    QMenu(),
    m_mouseArea(new DRegionMonitor(this))
{
    // NOTE(hualet): don't change those window flags, if you delete below line, deepin-menu
    // won't even show working with deepin-terminal2 and dde-launcher.
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::BypassWindowManagerHint | Qt::Tool);

    connect(m_mouseArea, &DRegionMonitor::buttonPress, this, [this] (const QPoint &p, const int) {
        if (!containsPoint(p)) {
            hide();
        }
    });
}

DDesktopMenu::~DDesktopMenu()
{
    m_mouseArea->unregisterRegion();
    m_mouseArea->deleteLater();
}

void DDesktopMenu::setItems(QJsonArray items)
{
    addActionFromJson(this, items);
}

void DDesktopMenu::setItemActivity(const QString &itemId, bool isActive)
{
    QAction *action = this->action(itemId);
    if (action) {
        action->setEnabled(isActive);
    }
}

void DDesktopMenu::setItemChecked(const QString &itemId, bool checked)
{
    QAction *action = this->action(itemId);
    if (action) {
        action->setChecked(checked);
    }
}

void DDesktopMenu::setItemText(const QString &itemId, const QString &text)
{
    QAction *action = this->action(itemId);
    if (action) {
        action->setText(text);
    }
}

void DDesktopMenu::grabFocus()
{
    QTimer::singleShot(100, this, [this] {
        activateWindow();
        grabMouse();
        grabKeyboard();
    });

    m_mouseArea->registerRegion();
}

void DDesktopMenu::keyPressEvent(QKeyEvent *event)
{
    int k=event->key();
    if (k == Qt::Key_Escape) {
        hide();
    }
    auto kayChar=QString(char(k));
    auto acts=m_ownMenus[0]->actions();

    for (QAction *act:acts) {
        if (kayChar == act->property("itemNavKey").toString()) {
            act->trigger();
        }
    }
    QMenu::keyPressEvent(event);
}

QAction *DDesktopMenu::action(const QString &id)
{
    for (QAction *action : actions()) {
        if (action->property("itemId") == id) {
            return action;
        }
    }

    return nullptr;
}

void DDesktopMenu::addActionFromJson(QMenu *menu, const QJsonArray &items)
{
    m_ownMenus << menu;

    foreach (QJsonValue item, items) {
        QJsonObject itemObj = item.toObject();
        QString itemText = static_cast<QString &&>(itemObj["itemText"].toString().replace("_", QString()).replace(QRegExp("\\([^)]+\\)"), QString()));
        const QString itemIcon = itemObj["itemIcon"].toString();

        const QJsonObject subMenuJson = itemObj["itemSubMenu"].toObject();
        const QJsonArray subMenuItemsJson = subMenuJson["items"].toArray();

        QAction *action = nullptr;
        if (subMenuItemsJson.count()) {
            QMenu *subMenu = new QMenu(menu);
            action = menu->addMenu(subMenu);
            addActionFromJson(subMenu, subMenuItemsJson);
        } else if (itemText.isEmpty()) {
            menu->addSeparator();
            continue;
        } else {
            action = new QAction(menu);
            menu->addAction(action);
        }

        action->setText(itemText);
        action->setIcon(QIcon(itemIcon));

        action->setEnabled(itemObj["isActive"].toBool());
        action->setCheckable(itemObj["isCheckable"].toBool());
        action->setChecked(itemObj["checked"].toBool());

        action->setProperty("itemId", itemObj["itemId"].toString());

        QRegExp regexp("_(.)");
        regexp.indexIn(itemObj["itemText"].toString());
        QString navKey = regexp.cap(1);
        QString navKeyWrapper = QString("%1").arg(navKey);
        itemText = itemObj["itemText"].toString().replace(regexp, navKeyWrapper);
        action->setText(itemText);
        action->setProperty("itemNavKey", navKey);

        connect(action, &QAction::triggered, this, [this, action] {
            const QString id = action->property("itemId").toString();
            emit itemClicked(id, action->isChecked());

            hide();
        });
    }
}

bool DDesktopMenu::containsPoint(const QPoint &point) const
{
    for (const QMenu *menu : m_ownMenus) {
        if (menu->geometry().contains(point))
            return true;
    }

    return false;
}
