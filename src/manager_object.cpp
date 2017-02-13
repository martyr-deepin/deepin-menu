/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QDBusObjectPath>
#include <QUuid>
#include <QDebug>

#include "dbus_menu_adaptor.h"
#include "manager_object.h"

ManagerObject::ManagerObject(QObject *parent) :
    QObject(parent)
{
    menuObjectPath = "";
    menuObject = NULL;
}

QDBusObjectPath ManagerObject::RegisterMenu()
{
    this->UnregisterMenu();

    QString uuid = QUuid::createUuid().toString();
    uuid = uuid.replace("{", "");
    uuid = uuid.replace("}", "");
    uuid = uuid.replace("-", "_");
    menuObjectPath = "/com/deepin/menu/" + uuid;

    menuObject = new MenuObject();
    menuAdaptor = new MenuAdaptor(menuObject);

    connect(menuObject, SIGNAL(destroyed()), this, SLOT(menuObjectDestroiedSlot()));

    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerObject(menuObjectPath, menuObject);

    QDBusObjectPath result(menuObjectPath);
    return result;
}

void ManagerObject::UnregisterMenu()
{
    if (menuObjectPath != "") {
        this->UnregisterMenu(menuObjectPath);
    }
}

void ManagerObject::UnregisterMenu(const QString &)
{
    if (menuObject) {
        menuObject->deleteLater();
    }
}

// private slots
void ManagerObject::menuObjectDestroiedSlot()
{
    menuObject = NULL;

    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.unregisterObject(menuObjectPath);
}
