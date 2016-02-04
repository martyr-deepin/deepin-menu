#! /usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright (C) 2015 Deepin Technology Co., Ltd.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.

from PyQt5.QtCore import pyqtSignal
from PyQt5.QtDBus import QDBusAbstractInterface, QDBusConnection, QDBusReply

class MenuManagerInterface(QDBusAbstractInterface):

    def __init__(self):
        super(MenuManagerInterface, self).__init__("com.deepin.menu",
                                                   "/com/deepin/menu",
                                                   "com.deepin.menu.Manager",
                                                   QDBusConnection.sessionBus(),
                                                   None)

    def registerMenu(self):
        return self.call('RegisterMenu')

    def unregisterMenu(self, objPath):
        self.call('UnregisterMenu', objPath)

class MenuObjectInterface(QDBusAbstractInterface):

    ItemInvoked = pyqtSignal(str, bool)
    ItemTextSet = pyqtSignal(str, str)
    ItemActivitySet = pyqtSignal(str, bool)
    ItemCheckedSet = pyqtSignal(str, bool)
    MenuUnregistered = pyqtSignal()

    def __init__(self, path):
        super(MenuObjectInterface, self).__init__("com.deepin.menu",
                                                  path,
                                                  "com.deepin.menu.Menu",
                                                  QDBusConnection.sessionBus(),
                                                  None)

    def showMenu(self, jsonContent):
        self.asyncCall('ShowMenu', jsonContent)

    def setItemText(self, id, value):
        self.asyncCall('SetItemText', id, value)

    def setItemActivity(self, id, value):
        self.asyncCall('SetItemActivity', id, value)

    def setItemChecked(self, id, value):
        self.asyncCall('SetItemChecked', id, value)

class XMouseAreaInterface(QDBusAbstractInterface):

    ButtonPress = pyqtSignal(int, int, int, int)
    KeyPress = pyqtSignal(str, int, int, int)
    MotionMove = pyqtSignal(int, int, int)

    def __init__(self):
        super(XMouseAreaInterface, self).__init__("com.deepin.api.XMouseArea",
                                                  "/com/deepin/api/XMouseArea",
                                                  "com.deepin.api.XMouseArea",
                                                  QDBusConnection.sessionBus(),
                                                  None)

    def registerArea(self, x1, x2, y1, y2, flag):
        msg = self.call("RegisterArea", x1, x2, y1, y2, flag)
        print msg.errorName(), msg.errorMessage()
        reply = QDBusReply(msg)
        return reply.value()

    def unregisterArea(self, id):
        self.call("UnregisterArea", id)

class DisplayPropertyInterface(QDBusAbstractInterface):
    def __init__(self):
        super(DisplayPropertyInterface, self).__init__(
            "com.deepin.daemon.Display",
            "/com/deepin/daemon/Display",
            "org.freedesktop.DBus.Properties",
            QDBusConnection.sessionBus(),
            None)

    def getPrimaryRect(self):
        msg = self.call("Get", "com.deepin.daemon.Display", "PrimaryRect")
        reply = QDBusReply(msg)
        return reply.value()
