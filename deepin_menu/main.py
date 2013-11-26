#! /usr/bin/python
# -*- coding: utf-8 -*-

from PyQt5 import QtCore
from PyQt5.QtWidgets import QApplication
from PyQt5.QtQuick import QQuickView
from PyQt5.QtGui import QSurfaceFormat, QColor, QCursor, QFont, QFontMetrics
from PyQt5.QtCore import QObject, Q_CLASSINFO, pyqtSlot, pyqtProperty, pyqtSignal
from PyQt5.QtDBus import QDBusAbstractAdaptor, QDBusConnection, QDBusMessage
import sys
import json
import signal

SCREEN_WIDTH = 0
SCREEN_HEIGHT = 0

def getCursorPosition():
    return QCursor().pos()

class MenuService(QObject):
    def __init__(self):
        super(MenuService, self).__init__()
        self.__dbusAdaptor = MenuServiceAdaptor(self)
        self._sessionBus = QDBusConnection.sessionBus()

        self.__count = -1

    def registerMenu(self):
        self.__count += 1
        objPath = "/com/deepin/menu/%s" % self.__count
        objPathHolder= objPath.replace("/", "_")
        setattr(self, objPathHolder, MenuObject(objPath))
        self._sessionBus.registerObject(objPath, getattr(self, objPathHolder))
        return objPath

    def unregisterMenu(self, objPath):
        self._sessionBus.unregisterObject(objPath)
        msg = QDBusMessage.createSignal('/com/deepin/menu', 'com.deepin.menu.Manager', 'MenuUnregistered')
        msg << objPath
        QDBusConnection.sessionBus().send(msg)

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
                '    <signal name="MenuUnregistered">\n'
                '      <arg direction="out" type="o" name="menuObjectPath"/>\n'
                '    </signal>\n'
                '  </interface>\n')

    MenuUnregistered = pyqtSignal(str)

    def __init__(self, parent):
        super(MenuServiceAdaptor, self).__init__(parent)

    @pyqtSlot(result=str)
    def RegisterMenu(self):
        return self.parent().registerMenu()

    @pyqtSlot(str)
    def UnregisterMenu(self, objPath):
        return self.parent().unregisterMenu(objPath)

class MenuObject(QObject):
    def __init__(self, objPath):
        super(MenuObject, self).__init__()
        self.__dbusAdaptor = MenuObjectAdaptor(self)
        self.objPath = objPath

    def showMenu(self, menuJsonContent):
        self.menu = Menu(self, menuJsonContent)
        self.menu.showMenu()
        self.menu.requestActivate()

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
                '  </interface>\n')

    ItemInvoked = pyqtSignal(str, bool)

    def __init__(self, parent):
        super(MenuObjectAdaptor, self).__init__(parent)

    @pyqtSlot(str)
    def ShowMenu(self, menuJsonContent):
        self.parent().showMenu(menuJsonContent)

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

class Menu(QQuickView):

    def __init__(self, dbusObj, menuJsonContent, parent=None):
        QQuickView.__init__(self)

        self.dbusObj = dbusObj
        self.parent = parent
        self.subMenu = None

        self.__menuJsonContent = menuJsonContent
        self.__injection = Injection()

    def focusOutEvent(self, e):
        if (self.parent and self.parent.isActive()) or (self.subMenu and self.subMenu.isActive()):
            return
        # self.destroyForward(True)

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
        msg = QDBusMessage.createSignal(self.dbusObj.objPath, 'com.deepin.menu.Menu', 'ItemInvoked')
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
            self.subMenu = Menu(self.dbusObj, menuJsonContent, self)
            self.subMenu.showMenu()
        else:
            self.subMenu = None

    def showMenu(self):
        qml_context = self.rootContext()
        qml_context.setContextProperty("_menu_view", self)
        qml_context.setContextProperty("_injection", self.__injection)

        surface_format = QSurfaceFormat()
        surface_format.setAlphaBufferSize(8)
        self.setFormat(surface_format)

        self.setColor(QColor(0, 0, 0, 0))
        self.setFlags(QtCore.Qt.Popup)
        if self.menuJsonObj["isDockMenu"] and not self.isSubMenu:
            self.setSource(QtCore.QUrl.fromLocalFile('DockMenu.qml'))
        else:
            self.setSource(QtCore.QUrl.fromLocalFile('RectMenu.qml'))

        self.setX(self.menuJsonObj["x"])
        self.setY(self.menuJsonObj["y"])

        self.show()

    @pyqtSlot(bool)
    def destroyBackward(self, includingSelf):
        if includingSelf: 
            self.destroy()
            if not self.parent:
                menuService.unregisterMenu(self.dbusObj.objPath)  
        else:
            self.requestActivate()
        if self.parent:
            self.parent.destroyBackward(True)

    @pyqtSlot(bool)
    def destroyForward(self, includingSelf):
        if includingSelf: 
            self.destroy()        
            if not self.parent:
                menuService.unregisterMenu(self.dbusObj.objPath)
        else:
            self.requestActivate()
        if self.subMenu:
            self.subMenu.destroyForward(True)

if __name__ == "__main__":
    app = QApplication(sys.argv)

    bus = QDBusConnection.sessionBus()
    menuService = MenuService()
    bus.registerService('com.deepin.menu')
    bus.registerObject('/com/deepin/menu', menuService)

    # retrieve the geometry of the screen
    desktopWidget = app.desktop()
    screenGeometry = desktopWidget.screenGeometry()
    SCREEN_WIDTH = screenGeometry.width()
    SCREEN_HEIGHT = screenGeometry.height()

    signal.signal(signal.SIGINT, signal.SIG_DFL)
    sys.exit(app.exec_())
