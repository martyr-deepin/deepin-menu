#! /usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright (C) 2011 ~ 2013 Deepin, Inc.
#               2011 ~ 2013 Wang YaoHua
# 
# Author:     Wang YaoHua <mr.asianwang@gmail.com>
# Maintainer: Wang YaoHua <mr.asianwang@gmail.com>
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

from PyQt5.QtCore import QCoreApplication
QCoreApplication.setAttribute(10, True)

import subprocess
from PyQt5.QtCore import QObject, Q_CLASSINFO, pyqtSlot, pyqtSignal
from PyQt5.QtDBus import QDBusAbstractAdaptor, QDBusAbstractInterface, QDBusConnection, QDBusMessage

class MenuService(QObject):
    def __init__(self):
        super(MenuService, self).__init__()
        self.__dbusAdaptor = MenuServiceAdaptor(self)
        self._sessionBus = QDBusConnection.sessionBus()

        self.__count = -1
        self.__menu_process = None

    def registerMenu(self):
        self.__count += 1
        objPath = "/com/deepin/menu/%s" % self.__count
        objPathHolder= objPath.replace("/", "_")
        setattr(self, objPathHolder, MenuObject(self, objPath))
        self._sessionBus.registerObject(objPath, getattr(self, objPathHolder))
        return objPath

    def unregisterMenu(self, objPath):
        self._sessionBus.unregisterObject(objPath)
        setattr(self, objPath.replace("/", "_"), None)
        msg = QDBusMessage.createSignal(objPath, 'com.deepin.menu.Menu', 'MenuUnregistered')
        QDBusConnection.sessionBus().send(msg)

    def showMenu(self, dbusPath, menuJsonContent):
        if self.__menu_process:
            self.__menu_process.kill()
            self.__menu_process = None
            #TODO send menu dismissed message here!
            self.unregisterMenu(dbusPath.replace(str(self.__count), str(self.__count - 1)))

        self.__menu_process = subprocess.Popen("python QuickMenu.py %s \'%s\'" % (dbusPath, menuJsonContent),
                                               stderr=subprocess.STDOUT,
                                               shell=True)        

class MenuServiceAdaptor(QDBusAbstractAdaptor):
    """DBus service for creating beautiful menus."""

    Q_CLASSINFO("D-Bus Interface", "com.deepin.menu.Manager")
    Q_CLASSINFO("D-Bus Introspection",
                '  <interface name="com.deepin.menu.Manager">\n'
                '    <method name="RegisterMenu">\n'
                '      <arg direction="out" type="o" name="menuObjectPath"/>\n'
                '    </method>\n'
                '    <method name="UnregisterMenu">\n'
                '      <arg direction="in" type="s" name="menuObjectPath"/>\n'
                '    </method>\n'
                '  </interface>\n')

    def __init__(self, parent):
        super(MenuServiceAdaptor, self).__init__(parent)

    @pyqtSlot(result=str)
    def RegisterMenu(self):
        return self.parent().registerMenu()

    @pyqtSlot(str)
    def UnregisterMenu(self, objPath):
        return self.parent().unregisterMenu(objPath)

class MenuObject(QObject):
    def __init__(self, manager, objPath):
        super(MenuObject, self).__init__()
        self.__dbusAdaptor = MenuObjectAdaptor(self)
        self.manager = manager
        self.objPath = objPath

    def showMenu(self, menuJsonContent):
        self.manager.showMenu(self.objPath, menuJsonContent)

class MenuObjectAdaptor(QDBusAbstractAdaptor):

    Q_CLASSINFO("D-Bus Interface", "com.deepin.menu.Menu")
    Q_CLASSINFO("D-Bus Introspection",
                '  <interface name="com.deepin.menu.Menu">\n'
                '    <method name="ShowMenu">\n'
                '      <arg direction="in" type="s" name="menuJsonContent"/>\n'
                '    </method>\n'
                '    <signal name="ItemInvoked">\n'
                '      <arg direction="out" type="s" name="itemId"/>\n'
                '      <arg direction="out" type="b" name="checked"/>\n'
                '    </signal>\n'
                '    <signal name="MenuUnregistered">\n'
                '    </signal>\n'
                '  </interface>\n')

    ItemInvoked = pyqtSignal(str, bool)
    MenuUnregistered = pyqtSignal()

    def __init__(self, parent):
        super(MenuObjectAdaptor, self).__init__(parent)

    @pyqtSlot(str)
    def ShowMenu(self, menuJsonContent):
        self.parent().showMenu(menuJsonContent)
        
class MenuManagerInterface(QDBusAbstractInterface):

    def __init__(self):
        super(MenuManagerInterface, self).__init__("com.deepin.menu",
                                                   "/com/deepin/menu",
                                                   "com.deepin.menu.Manager",
                                                   QDBusConnection.sessionBus(), None)

    def registerMenu(self):
        return self.call('RegisterMenu')

    def unregisterMenu(self, objPath):
        self.call('UnregisterMenu', objPath)
        
