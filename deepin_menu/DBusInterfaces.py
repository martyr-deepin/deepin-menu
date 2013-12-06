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

from PyQt5.QtCore import pyqtSignal
from PyQt5.QtDBus import QDBusAbstractInterface, QDBusConnection

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

class MenuObjectInterface(QDBusAbstractInterface):

    ItemInvoked = pyqtSignal(str, bool)
    ItemActivitySet = pyqtSignal(str, bool)
    MenuUnregistered = pyqtSignal()

    def __init__(self, path):
        super(MenuObjectInterface, self).__init__("com.deepin.menu",
                                                  path,
                                                  "com.deepin.menu.Menu",
                                                  QDBusConnection.sessionBus(), None)

    def showMenu(self, jsonContent):
        self.call('ShowMenu', jsonContent)
        
    def setItemActivity(self, id, value):
        self.call('SetItemActivity', id, value)
