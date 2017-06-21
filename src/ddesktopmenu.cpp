/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "ddesktopmenu.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QKeyEvent>

DDesktopMenu::DDesktopMenu() :
    QMenu(),
    m_mouseArea(new XMouseArea("com.deepin.api.XMouseArea", "/com/deepin/api/XMouseArea", QDBusConnection::sessionBus(), this))
{
    // NOTE(hualet): don't change those window flags, if you delete below line, deepin-menu
    // won't even show working with deepin-terminal2 and dde-launcher.
    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::BypassWindowManagerHint | Qt::Tool);

    connect(m_mouseArea, &__XMouseArea::ButtonPress, this, [this] (int, int x, int y, const QString &key) {
        if (key == m_key && !containsPoint(QPoint(x, y))) {
            hide();
        }
    });
}

DDesktopMenu::~DDesktopMenu()
{
    m_mouseArea->UnregisterArea(m_key);
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

    QDBusPendingCall call = m_mouseArea->RegisterFullScreen();
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, [this, watcher] {
        if (watcher->isError()) {
            qWarning() << "error registering mouse area: " << watcher->error().message();
        } else {
            QDBusReply<QString> reply = watcher->reply();
            m_key = reply.value();
        }
    });
}

void DDesktopMenu::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        hide();
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
        const QString itemText = itemObj["itemText"].toString().replace("_", QString()).replace(QRegExp("\\([^)]+\\)"), QString());
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
