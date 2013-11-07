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

from PyQt5.QtCore import (QCoreApplication, QObject, Q_CLASSINFO, pyqtSlot)
from PyQt5.QtDBus import QDBusConnection, QDBusAbstractAdaptor

class MenuService(QObject):
    def __init__(self):
        super(MenuService, self).__init__()
        self.__dbusAdaptor = MenuServiceAdaptor(self)
        
    def showMenu(self, x, y, content):
        return "helloworld"

class MenuServiceAdaptor(QDBusAbstractAdaptor):
    """DBus service for creating beautiful menus."""

    Q_CLASSINFO("D-Bus Interface", "com.deepin.menu.Menu")
    Q_CLASSINFO("D-Bus Introspection",
    '  <interface name="com.deepin.menu.Menu">\n'
    '    <method name="ShowMenu">\n'
    '      <arg direction="in" type="i" name="x"/>\n'
    '      <arg direction="in" type="i" name="y"/>\n'
    '      <arg direction="in" type="s" name="content"/>\n'
    '      <arg direction="out" type="s" name="out"/>\n'
    '    </method>\n'
    '  </interface>\n')

    def __init__(self, parent):
        super(MenuServiceAdaptor, self).__init__(parent)

    @pyqtSlot(int, int, str, result=str)
    def ShowMenu(self, x, y, content):
        return self.parent().showMenu(x, y, content)

def start():
    app = QCoreApplication([])
    bus = QDBusConnection.sessionBus()
    menuService = MenuService()
    bus.registerObject('/com/deepin/menu', menuService)
    bus.registerService('com.deepin.menu')
    app.exec_()

if __name__ == '__main__':
    start()
