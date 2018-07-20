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

/*
 * This file was generated by qdbusxml2cpp version 0.8
 * Command line was: qdbusxml2cpp -c ManagerAdaptor -a dbus_manager_adaptor.h:dbus_manager_adaptor.cpp com.deepin.menu.Manager.xml
 *
 * qdbusxml2cpp is Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#ifndef DBUS_MANAGER_ADAPTOR_H_1405666337
#define DBUS_MANAGER_ADAPTOR_H_1405666337

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
QT_BEGIN_NAMESPACE
class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;
QT_END_NAMESPACE

/*
 * Adaptor class for interface com.deepin.menu.Manager
 */
class ManagerAdaptor: public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.menu.Manager")
    Q_CLASSINFO("D-Bus Introspection", ""
"  <interface name=\"com.deepin.menu.Manager\">\n"
"    <method name=\"RegisterMenu\">\n"
"      <arg direction=\"out\" type=\"o\" name=\"menuObjectPath\"/>\n"
"    </method>\n"
"    <method name=\"UnregisterMenu\">\n"
"      <arg direction=\"in\" type=\"s\" name=\"menuObjectPath\"/>\n"
"    </method>\n"
"  </interface>\n"
        "")
public:
    ManagerAdaptor(QObject *parent);
    virtual ~ManagerAdaptor();

public: // PROPERTIES
public Q_SLOTS: // METHODS
    QDBusObjectPath RegisterMenu();
    void UnregisterMenu(const QString &menuObjectPath);
Q_SIGNALS: // SIGNALS
};

#endif