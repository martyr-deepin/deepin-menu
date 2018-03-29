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

#ifndef MANAGER_OBJECT_H
#define MANAGER_OBJECT_H

#include <QObject>
#include <QString>
#include <QDBusObjectPath>
#include <QMutex>

#include <src/dbus_menu_adaptor.h>
#include <src/menu_object.h>

class ManagerObject : public QObject
{
    Q_OBJECT
public:
    explicit ManagerObject(QObject *parent = 0);

signals:

public slots:
    QDBusObjectPath RegisterMenu();
    void UnregisterMenu();
    void UnregisterMenu(const QString &menuObjectPath);

private:
    QMutex menuRegisterGuard;
    QPointer<MenuObject> menuObject;
    QPointer<MenuAdaptor> menuAdaptor;
    QString menuObjectPath;

private slots:
    void menuObjectDestroiedSlot();
};

#endif // MANAGER_OBJECT_H
