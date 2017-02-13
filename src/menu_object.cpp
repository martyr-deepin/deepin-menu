/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QColor>
#include <QGuiApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtGlobal>
#include <QDebug>

#include "menu_object.h"
#include "ddesktopmenu.h"
#include "ddockmenu.h"

static DDockMenu::Direction DirectionFromString(QString direction) {
    if (direction == "top") {
        return DDockMenu::Top;
    } else if (direction == "left") {
        return DDockMenu::Left;
    } else if (direction == "right") {
        return DDockMenu::Right;
    }

    return DDockMenu::Bottom;
}

MenuObject::MenuObject():
    QObject()
{
    _menu = NULL;
}

MenuObject::~MenuObject()
{
    if(_menu) {
        _menu->deleteLater();
    }

    emit MenuUnregistered();
}

void MenuObject::SetItemActivity(const QString &itemId, bool isActive)
{
    if (_menu) _menu->setItemActivity(itemId, isActive);
}

void MenuObject::SetItemChecked(const QString &itemId, bool checked)
{
    if (_menu) _menu->setItemChecked(itemId, checked);
}

void MenuObject::SetItemText(const QString &itemId, const QString &text)
{
    if (_menu) _menu->setItemText(itemId, text);
}

void MenuObject::ShowMenu(const QString &menuJsonContent)
{
    Q_ASSERT(_menu == NULL);

    QByteArray bytes;
    bytes.append(menuJsonContent);
    QJsonDocument menuDocument = QJsonDocument::fromJson(bytes);
    QJsonObject jsonObj = menuDocument.object();

    if(jsonObj["isDockMenu"].toBool()) {
        _menu = new DDockMenu();
    } else {
        _menu = new DDesktopMenu();
    }

    connect(_menu, SIGNAL(destroyed()), this, SLOT(menuDismissedSlot()));
    connect(_menu, SIGNAL(itemClicked(QString,bool)), this, SIGNAL(ItemInvoked(QString,bool)));

    bytes.clear();
    bytes.append(jsonObj["menuJsonContent"].toString());
    QJsonDocument menuContent = QJsonDocument::fromJson(bytes);
    QJsonObject menuContentObj = menuContent.object();

    int x = jsonObj["x"].toDouble();
    int y = jsonObj["y"].toDouble();
    QString direction = jsonObj["direction"].toString();

    DDockMenu * dm = qobject_cast<DDockMenu*>(_menu);
    if (dm) {
        dm->setDirection(DirectionFromString(direction));
    }

    _menu->setContent(menuContentObj["items"].toArray());
    _menu->setPosition(x, y);
    _menu->show();
    _menu->grabFocus();
}

void MenuObject::menuDismissedSlot()
{
    _menu = NULL;

    this->deleteLater();
}
