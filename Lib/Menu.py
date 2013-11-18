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

import json

from PyQt5.QtCore import pyqtSlot, pyqtSignal
from PyQt5.QtDBus import QDBusAbstractInterface, QDBusConnection

idIndex = 0

DBUS_SERVICE = "com.deepin.menu"
DBUS_PATH = "/com/deepin/menu"
DBUS_INTERFACE = "com.deepin.menu.Menu"

class MenuServiceInterface(QDBusAbstractInterface):

    ItemInvoked = pyqtSignal(int)

    def __init__(self, ):
        super(MenuServiceInterface, self).__init__(DBUS_SERVICE, DBUS_PATH, DBUS_INTERFACE,
                                                   QDBusConnection.sessionBus(), None)

    def showMenu(self, x, y, content):
        self.call('ShowMenu', x, y, content, True)

class MenuItem(object):
    def __init__(self, text, icon=None, node=None, *rest):
        self.id = None

        self.text = text or ""
        self.icon = icon or ""
        self.node = node or Menu()
        
        if isinstance(self.node, Menu):
            self.subMenu = self.node
        else:
            self.callback = self.node
            self.subMenu = Menu()
        
        self.rest = rest

    @property
    def serializableContent(self):
        return {"itemId": self.id,
                "itemIcon": self.icon,
                "itemText": self.text,
                "itemSubMenu": self.subMenu.serializableItemList }

    def setSubMenu(self, menu):
        self.subMenu = menu

    def setCallBack(self, callback):
        self.call = callback

    def __str__(self):
        return json.dumps(self.serializableContent)

class MenuSeparator(object):
    
    def __init__(self):
        self.callback = None
    
    @property
    def serializableContent(self):
        return {"itemId": self.id,
                "itemIcon": "",
                "itemText": "",
                "itemSubMenu": "[]"}

class Menu(object):

    def __init__(self, items=None, is_root=False):
        # self.__items = items or []
        self.items = []
        if items:
            self.addMenuItems(items)
        if is_root:
            self.iface = MenuServiceInterface()

    @property
    def serializableItemList(self):
        result = []
        for item in self.items:
            result.append(item.serializableContent)
        return result

    def addMenuItem(self, item):
        global idIndex
        idIndex += 1
        item.id = idIndex
        self.items.append(item)

    def addMenuItems(self, items):
        for item in items:
            self.addMenuItem(item)

    def show(self, x, y):
        self.iface.showMenu(x, y, str(self))
        self.iface.ItemInvoked.connect(self.itemInvokedSlot)

    @pyqtSlot(int)
    def itemInvokedSlot(self, itemId):
        for item in self.items:
            if item.id == itemId:
                if hasattr(item, "callback"):
                    item.callback(item.rest)

    def __str__(self):
        return json.dumps(self.serializableItemList)

if __name__ == "__main__":
    import sys
    from PyQt5.QtCore import QCoreApplication

    app = QCoreApplication([])
    
    def test(x):
        print x
    
    driver = MenuItem("Driver", "/usr/share/icons/Deepin/apps/16/preferences-driver.png", test, "hello")
    display = MenuItem("Display", "/usr/share/icons/Deepin/apps/16/preferences-display.png")
    show = Menu([MenuItem("Display", "/usr/share/icons/Deepin/apps/16/preferences-display.png")])
    display.setSubMenu(show)
    menu = Menu([driver, MenuSeparator(), display], True)
    menu.show(200, 200)

    sys.exit(app.exec_())
