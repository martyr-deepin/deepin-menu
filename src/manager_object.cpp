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
    menuAdaptor = nullptr;
}

QDBusObjectPath ManagerObject::RegisterMenu()
{
    this->UnregisterMenu();

    if (menuObject)
        menuObject->deleteLater();

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
