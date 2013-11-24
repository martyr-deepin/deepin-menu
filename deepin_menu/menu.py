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

from PyQt5.QtGui import QColor
from PyQt5.QtCore import QObject, pyqtSlot, pyqtSignal
from PyQt5.QtDBus import QDBusAbstractInterface, QDBusConnection, QDBusReply

def parseMenuItem(menuItem):
    assert len(menuItem) >= 2
    result = MenuItem(menuItem[0], menuItem[1])
    if len(menuItem) > 2:
        result.setIcons(menuItem[2])
    if len(menuItem) > 3:
        result.setSubMenu(parseMenu(Menu(is_root=False), menuItem[3]))
    return result

def parseMenu(obj, menu):
    result = obj
    for menuItem in menu:
        if menuItem == None:
            result.addMenuItem(MenuSeparator())
        elif isinstance(menuItem, MenuItem):
            result.addMenuItem(menuItem)
        else:
            result.addMenuItem(parseMenuItem(menuItem))
    return result

def getRgbaF(r, g, b, a):
    color = QColor()
    color.setRgbF(r, g, b, a)
    return color.name()

class MenuManagerInterface(QDBusAbstractInterface):

    MenuUnregistered = pyqtSignal(str)

    def __init__(self):
        super(MenuManagerInterface, self).__init__("com.deepin.menu",
                                                   "/com/deepin/menu",
                                                   "com.deepin.menu.Manager",
                                                   QDBusConnection.sessionBus(), None)

    def registerMenu(self):
        return self.call('RegisterMenu')

    def unregisterMenu(self, objPath):
        self.call('UnregisterMenu', objPath)

class MenuObjectInterface(QDBusAbstractInterface):

    ItemInvoked = pyqtSignal(str)

    def __init__(self, path):
        super(MenuObjectInterface, self).__init__("com.deepin.menu",
                                                  path,
                                                  "com.deepin.menu.Menu",
                                                  QDBusConnection.sessionBus(), None)

    def showMenu(self, jsonContent):
        self.call('ShowMenu', jsonContent)

class MenuItem(QObject):
    def __init__(self, id, text, icons=None, subMenu=None, isActive=True, isCheckable=False, checked=False):
        super(MenuItem, self).__init__()
        self.id = id
        self.text = text
        self.icons = icons or ()
        self.subMenu = subMenu or Menu(is_root=False)
        self.isActive = isActive
        self.isCheckable = isCheckable
        self.checked = checked

    @property
    def serializableContent(self):
        iconNormal = ""
        iconHover = ""

        if len(self.icons) > 0:
            iconNormal = self.icons[0]
            iconHover = self.icons[0]
        if len(self.icons) > 1:
            iconHover = self.icons[1]

        return {"itemId": self.id,
                "itemIcon": iconNormal,
                "itemIconHover": iconHover,
                "itemText": self.text,
                "itemSubMenu": self.subMenu.serializableItemList,
                "isActive": self.isActive,
                "isCheckable": self.isCheckable,
                "checked": self.checked}

    def setSubMenu(self, menu):
        self.subMenu = menu

    def setIcons(self, icons):
        self.icons = icons

    @property
    def hasSubMenu(self):
        return len(self.subMenu.items) != 0

    def __str__(self):
        return json.dumps(self.serializableContent)

class CheckboxMenuItem(MenuItem):
    def __init__(self, id, text, checked=False):
        super(CheckboxMenuItem, self).__init__(id, text, isCheckable=True, checked=checked)

class MenuSeparator(MenuItem):
    def __init__(self):
        super(MenuSeparator, self).__init__("", "", isActive=False)

class Menu(QObject):
    itemClicked = pyqtSignal(str)

    def __init__(self, items=None, is_root=True, checkableMenu=False, singleCheck=False):
        super(Menu, self).__init__()
        self.items = []
        if items:
            parseMenu(self, items)
        if is_root:
            self.managerIface = MenuManagerInterface()
        self.checkableMenu = checkableMenu
        self.singleCheck = singleCheck

    @property
    def serializableItemList(self):
        items= []
        for item in self.items:
            items.append(item.serializableContent)
        return {"items": items,
                "checkableMenu": self.checkableMenu,
                "singleCheck": self.singleCheck}

    def addMenuItem(self, item):
        self.items.append(item)

    def addMenuItems(self, items):
        for item in items:
            self.addMenuItem(item)

    def getItemById(self, id):
        for item in self.items:
            if item.hasSubMenu:
                if item.subMenu.getItemById(id):
                    return item.subMenu.getItemById(id)
            else:
                if item.id == id:
                    return item
        return None

    def showRectMenu(self, x, y):
        msg = self.managerIface.registerMenu()
        reply = QDBusReply(msg)
        self.menuIface = MenuObjectInterface(reply.value())
        self.menuIface.showMenu(json.dumps({"x": x,
                                            "y": y,
                                            "fillColor": "#00000099",
                                            "fontColor": "#000000",
                                            "fontColorHover": "000000",
                                            "fontColorNotActive": "#b4b4b4",
                                            "borderColor": "#00000026",
                                            "blurRadius": 16,
                                            "isDockMenu": False,
                                            "menuJsonContent": str(self)}))
        self.menuIface.ItemInvoked.connect(self.itemInvokedSlot)

    def showDockMenu(self, x, y, cornerDirection="down"):
        msg = self.managerIface.registerMenu()
        reply = QDBusReply(msg)
        self.menuIface = MenuObjectInterface(reply.value())
        self.menuIface.showMenu(json.dumps({"x": x,
                                            "y": y,
                                            "fillColor": "#FFFFFF99",
                                            "fontColor": "#FFFFFF",
                                            "fontColorHover": "#00A4E2",
                                            "fontColorNotActive": "#646464",
                                            "borderColor": "#00000026",
                                            "blurRadius": 16,
                                            "isDockMenu": True,
                                            "cornerDirection": cornerDirection,
                                            "menuJsonContent": str(self)}))
        self.menuIface.ItemInvoked.connect(self.itemInvokedSlot)

    @pyqtSlot(str)
    def itemInvokedSlot(self, itemId):
        self.itemClicked.emit(itemId)

    def __str__(self):
        return json.dumps(self.serializableItemList)

class CheckboxMenu(Menu):
    def __init__(self, items):
        super(CheckboxMenu, self).__init__(items, checkableMenu=True, singleCheck=False)

    def addMenuItem(self, item):
        item.isCheckable = True
        self.items.append(item)

class RadioButtonMenu(Menu):
    def __init__(self, items):
        super(RadioButtonMenu, self).__init__(items, checkableMenu=True, singleCheck=True)

    def addMenuItem(self, item):
        item.isCheckable = True
        self.items.append(item)

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
    menu = Menu([("id_driver", "Driver", ("/usr/share/icons/Deepin/apps/16/preferences-display.png",), [("id_sub1", "SubMenu1"), ("id_sub2", "SubMenu2")]),
                 None,
                 ("id_display", "Display"),
                 ("id_checkbox", "CheckBoxMenu"),
                 MenuSeparator(),
                 MenuItem("id_nonactive", "NotActive", isActive=False),
                 CheckboxMenuItem("id_check", "CheckMe", True)], is_root=True,)
    sub = RadioButtonMenu([("id_radio1", "Radio1"), ("id_radio2", "Radio2"),])
    menu.getItemById("id_checkbox").setSubMenu(sub)
    menu.itemClicked.connect(test)
    # menu.showRectMenu(300, 300)
    menu.showDockMenu(300, 300)

    sys.exit(app.exec_())
