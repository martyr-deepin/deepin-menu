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

from PyQt5 import QtCore
from PyQt5.QtWidgets import QApplication, qApp
from PyQt5.QtQuick import QQuickView
from PyQt5.QtGui import QSurfaceFormat, QColor, QKeySequence, QCursor, QFont, QFontMetrics
from PyQt5.QtCore import QObject, pyqtSlot, pyqtProperty
from PyQt5.QtDBus import QDBusConnection, QDBusMessage
import os
import sys
import json
import signal
from DBusBackend import MenuManagerInterface

SCREEN_WIDTH = 0
SCREEN_HEIGHT = 0

def getCursorPosition():
    return QCursor().pos()

class Injection(QObject):
    def __init__(self):
        super(QObject, self).__init__()

    @pyqtSlot(result=int)
    def getScreenWidth(self):
        return SCREEN_WIDTH

    @pyqtSlot(result=int)
    def getScreenHeight(self):
        return SCREEN_HEIGHT

    @pyqtSlot(str,int,result=int)
    def getStringWidth(self, string, pixelSize):
        font = QFont()
        font.setPixelSize(pixelSize)
        fm = QFontMetrics(font)
        return fm.width(string)

    @pyqtSlot(str,int,result=int)
    def getStringHeight(self, string, pixelSize):
        font = QFont()
        font.setPixelSize(pixelSize)
        fm = QFontMetrics(font)
        return fm.height()

    @pyqtSlot(str, result=int)
    def keyStringToCode(self, s):
        seq = QKeySequence(s)
        if len(seq) > 0:
            return seq[0]
        return -1

class Menu(QQuickView):

    def __init__(self, dbusPath, menuJsonContent, parent=None):
        QQuickView.__init__(self)

        self.dbusPath = dbusPath
        self.parent = parent
        self.subMenu = None

        self.__menuJsonContent = menuJsonContent
        self.__injection = Injection()
        qApp.focusWindowChanged.connect(self.focusWindowChangedSlot)

    def setDBusPath(self, dbusPath):
        self.dbusPath = dbusPath

    def setMenuJsonContent(self, menuJsonContent):
        self.__menuJsonContent = menuJsonContent

    def focusWindowChangedSlot(self, window):
        if not self:
            return
        if window == None:
            self.destroyForward(True)

    @pyqtProperty(bool)
    def isSubMenu(self):
        return not self.parent == None

    @pyqtProperty(str)
    def menuJsonContent(self):
        return self.__menuJsonContent

    @pyqtProperty("QVariant", constant=True)
    def menuJsonObj(self):
        return json.loads(self.__menuJsonContent)

    @pyqtSlot(str, bool)
    def updateCheckableItem(self, id, value):
        self.rootObject().updateCheckableItem(id, value)
        if self.parent:
            self.parent.updateCheckableItem(id, value)

    @pyqtSlot(str, bool)
    def invokeItem(self, id, checked):
        msg = QDBusMessage.createSignal(self.dbusPath, 'com.deepin.menu.Menu', 'ItemInvoked')
        msg << id << checked
        QDBusConnection.sessionBus().send(msg)

    @pyqtSlot()
    def activateParent(self):
        if self.parent != None:
            self.parent.requestActivate()

    @pyqtSlot()
    def activateSubMenu(self):
        if self.subMenu != None:
            self.subMenu.requestActivate()
            self.subMenu.rootObject().selectItem(0)

    @pyqtSlot(str)
    def showSubMenu(self, menuJsonContent):
        if menuJsonContent:
            self.subMenu = Menu(self.dbusPath, menuJsonContent, self)
            self.subMenu.showMenu()
        else:
            self.subMenu = None

    def showMenu(self):
        qml_context = self.rootContext()
        qml_context.setContextProperty("_application", qApp)
        qml_context.setContextProperty("_menu_view", self)
        qml_context.setContextProperty("_injection", self.__injection)

        surface_format = QSurfaceFormat()
        surface_format.setAlphaBufferSize(8)
        self.setFormat(surface_format)

        self.setColor(QColor(0, 0, 0, 0))
        self.setFlags(QtCore.Qt.FramelessWindowHint | QtCore.Qt.Tool)
        self.setX(self.menuJsonObj["x"])
        self.setY(self.menuJsonObj["y"])

        if self.menuJsonObj["isDockMenu"] and not self.isSubMenu:
            self.setSource(QtCore.QUrl.fromLocalFile(os.path.join(os.path.dirname(__file__), 'DockMenu.qml')))
        else:
            self.setSource(QtCore.QUrl.fromLocalFile(os.path.join(os.path.dirname(__file__), 'RectMenu.qml')))

        self.show()
    
    @pyqtSlot()
    def unregisterMenu(self):
        menuService.unregisterMenu(self.dbusPath)

    @pyqtSlot(bool)
    def destroyForward(self, includingSelf):
        if self.subMenu:
            self.subMenu.destroyForward(True)
        if includingSelf:
            if not self.parent:
                menuService.unregisterMenu(self.dbusPath)
            self.destroy()
        else:
            self.requestActivate()

if __name__ == "__main__":
    app = QApplication([])

    # retrieve the geometry of the screen
    desktopWidget = app.desktop()
    screenGeometry = desktopWidget.screenGeometry()
    SCREEN_WIDTH = screenGeometry.width()
    SCREEN_HEIGHT = screenGeometry.height()
    
    menuService = MenuManagerInterface()
    
    menu = Menu(sys.argv[1], sys.argv[2])
    menu.showMenu()
    menu.requestActivate()

    signal.signal(signal.SIGINT, signal.SIG_DFL)
    sys.exit(app.exec_())
