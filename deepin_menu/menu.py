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

from PyQt5.QtCore import QObject, pyqtSlot, pyqtSignal
from PyQt5.QtDBus import QDBusAbstractInterface, QDBusConnection

idIndex = 0

DBUS_SERVICE = "com.deepin.menu"
DBUS_PATH = "/com/deepin/menu"
DBUS_INTERFACE = "com.deepin.menu.Menu"

def parseMenuItem(menuItem):
    assert len(menuItem) >= 2
    result = MenuItem(menuItem[0], menuItem[1])
    if len(menuItem) > 2:
        result.setSubMenu(parseMenu(Menu(is_root=False), menuItem[2]))
    return result

def parseMenu(obj, menu):
    result = obj
    for menuItem in menu:
        if menuItem == None:
            result.addMenuItem(MenuSeparator())
        else:
            result.addMenuItem(parseMenuItem(menuItem))
    return result

class MenuServiceInterface(QDBusAbstractInterface):

    ItemInvoked = pyqtSignal(str)

    def __init__(self, ):
        super(MenuServiceInterface, self).__init__(DBUS_SERVICE, DBUS_PATH, DBUS_INTERFACE,
                                                   QDBusConnection.sessionBus(), None)

    def showMenu(self, x, y, content):
        self.call('ShowMenu', x, y, content)

    def showDockMenu(self, x, y, content):
        self.call('ShowDockMenu', x, y, content)

class MenuItem(QObject):
    def __init__(self, id, text, icon=None, subMenu=None):
        super(MenuItem, self).__init__()
        self.id = id
        self.text = text
        self.icon = icon or ""
        self.subMenu = subMenu or Menu(is_root=False)

    @property
    def serializableContent(self):
        return {"itemId": self.id,
                "itemIcon": self.icon,
                "itemText": self.text,
                "itemSubMenu": self.subMenu.serializableItemList }

    def setSubMenu(self, menu):
        self.subMenu = menu

    def __str__(self):
        return json.dumps(self.serializableContent)

class MenuSeparator(QObject):
    def __init__(self):
        super(MenuSeparator, self).__init__()

    @property
    def serializableContent(self):
        return {"itemId": "",
                "itemIcon": "",
                "itemText": "",
                "itemSubMenu": "[]"}

class Menu(QObject):
    itemClicked = pyqtSignal(str)

    def __init__(self, items=None, is_root=True):
        super(Menu, self).__init__()
        self.items = []
        if items:
            parseMenu(self, items)
        if is_root:
            self.iface = MenuServiceInterface()

    @property
    def serializableItemList(self):
        result = []
        for item in self.items:
            result.append(item.serializableContent)
        return result

    def addMenuItem(self, item):
        self.items.append(item)

    def addMenuItems(self, items):
        for item in items:
            self.addMenuItem(item)

    def showMenu(self, x, y):
        self.iface.showMenu(x, y, str(self))
        self.iface.ItemInvoked.connect(self.itemInvokedSlot)

    def showDockMenu(self, x, y):
        self.iface.showDockMenu(x, y, str(self))
        self.iface.ItemInvoked.connect(self.itemInvokedSlot)

    @pyqtSlot(str)
    def itemInvokedSlot(self, itemId):
        print "emit", itemId
        self.itemClicked.emit(itemId)

    def __str__(self):
        return json.dumps(self.serializableItemList)

if __name__ == "__main__":
    import sys
    from PyQt5.QtCore import QCoreApplication

    app = QCoreApplication([])

    @pyqtSlot(str)
    def test(s):
        print "test", s

    # 1)
    # driver = MenuItem("id_driver", "Driver", "/usr/share/icons/Deepin/apps/16/preferences-driver.png")
    # display = MenuItem("id_display", "Display", "/usr/share/icons/Deepin/apps/16/preferences-display.png")
    # show = Menu()
    # show.addMenuItem(MenuItem("id_sub_display", "Display", "/usr/share/icons/Deepin/apps/16/preferences-display.png"))
    # display.setSubMenu(show)
    # menu = Menu(is_root=True)
    # menu.addMenuItems([driver, display])
    # menu.showMenu(200, 200)

    # 2)
    menu = Menu([("id_driver", "Driver", [("id_sub1", "SubMenu1"), ("id_sub2", "SubMenu2")]),
                 None, 
                 ("id_display", "Display")], is_root=True)
    menu.itemClicked.connect(test)
    menu.showDockMenu(200, 300)

    sys.exit(app.exec_())
