#! /usr/bin/python
# -*- coding: utf-8 -*-
from PyQt5.QtCore import QCoreApplication
QCoreApplication.setAttribute(10, True)

from PyQt5 import QtCore
from PyQt5.QtWidgets import QApplication, qApp
from PyQt5.QtQuick import QQuickView
from PyQt5.QtGui import QSurfaceFormat, QColor, QKeySequence, QCursor, QFont, QFontMetrics
from PyQt5.QtCore import QObject, Q_CLASSINFO, pyqtSlot, pyqtProperty, pyqtSignal
from PyQt5.QtDBus import QDBusAbstractAdaptor, QDBusConnection, QDBusConnectionInterface, QDBusMessage
import os
import sys
import json
import signal
import subprocess
from uuid import uuid4

SCREEN_WIDTH = 0
SCREEN_HEIGHT = 0

def getCursorPosition():
    return QCursor().pos()

class MenuService(QObject):
    def __init__(self):
        super(MenuService, self).__init__()
        self.__dbusAdaptor = MenuServiceAdaptor(self)
        self._sessionBus = QDBusConnection.sessionBus()

        self.__menu = None

    def registerMenu(self):
        objPath = "/com/deepin/menu/%s" % str(uuid4()).replace("-", "_")
        objPathHolder= objPath.replace("/", "_")
        setattr(self, objPathHolder, MenuObject(self, objPath))
        self._sessionBus.registerObject(objPath, getattr(self, objPathHolder))
        return objPath

    def unregisterMenu(self, objPath):
        self._sessionBus.unregisterObject(objPath)
        setattr(self, objPath.replace("/", "_"), None)
        msg = QDBusMessage.createSignal(objPath, 'com.deepin.menu.Menu', 'MenuUnregistered')
        QDBusConnection.sessionBus().send(msg)
        
        INJECTION.startNewService() 

    def showMenu(self, dbusObj, menuJsonContent):
        print os.getpid()
        if self.__menu:
            self.__menu.destroyForward(False)
            self.__menu.setDBusObj(dbusObj)
            self.__menu.setMenuJsonContent(menuJsonContent)
        else:
            self.__menu = Menu(dbusObj, menuJsonContent)

        self.__menu.showMenu()
        self.__menu.requestActivate()

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
        self.manager.showMenu(self, menuJsonContent)

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
    
    @pyqtSlot()
    def startNewService(self):
        subprocess.Popen(["python", "-OO", "main.py"])
        
INJECTION = Injection()

class Menu(QQuickView):

    def __init__(self, dbusObj, menuJsonContent, parent=None):
        QQuickView.__init__(self)

        self.dbusObj = dbusObj
        self.parent = parent
        self.subMenu = None

        self.__menuJsonContent = menuJsonContent
        qApp.focusWindowChanged.connect(self.focusWindowChangedSlot)

    def setDBusObj(self, dbusObj):
        self.dbusObj = dbusObj

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
        qml_context.setContextProperty("_injection", INJECTION)

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

    @pyqtSlot(bool)
    def destroyBackward(self, includingSelf):
        self.engine().collectGarbage()
        if self.parent:
            self.parent.destroyBackward(True)
        if includingSelf:
            if not self.parent:
                menuService.unregisterMenu(self.dbusObj.objPath)
            self.close()
        else:
            self.requestActivate()

    @pyqtSlot(bool)
    def destroyForward(self, includingSelf):
        self.engine().collectGarbage()            
        if self.subMenu:
            self.subMenu.destroyForward(True)
        if includingSelf:
            if not self.parent:
                menuService.unregisterMenu(self.dbusObj.objPath)
            # self.deleteLater()
            self.close()
        else:
            self.requestActivate()
            
@pyqtSlot(str)
def serviceReplacedByOtherSlot(name):
    sys.exit(0)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    app.setQuitOnLastWindowClosed(False)

    bus = QDBusConnection.sessionBus()
    menuService = MenuService()
    bus.interface().registerService('com.deepin.menu', 
                                    QDBusConnectionInterface.ReplaceExistingService, 
                                    QDBusConnectionInterface.AllowReplacement)
    bus.registerObject('/com/deepin/menu', menuService)
    bus.interface().serviceUnregistered.connect(serviceReplacedByOtherSlot)

    # retrieve the geometry of the screen
    desktopWidget = app.desktop()
    screenGeometry = desktopWidget.screenGeometry()
    SCREEN_WIDTH = screenGeometry.width()
    SCREEN_HEIGHT = screenGeometry.height()

    signal.signal(signal.SIGINT, signal.SIG_DFL)
    sys.exit(app.exec_())
