/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef MANAGER_OBJECT_H
#define MANAGER_OBJECT_H

#include <QObject>
#include <QString>
#include <QDBusObjectPath>

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
    MenuObject *menuObject;
    MenuAdaptor *menuAdaptor;
    QString menuObjectPath;

private slots:
    void menuObjectDestroiedSlot();
};

#endif // MANAGER_OBJECT_H
