#! /usr/bin/python
# -*- coding: utf-8 -*-
from PyQt5 import QtCore
from PyQt5.QtCore import QCoreApplication
QCoreApplication.setAttribute(10, True)
QCoreApplication.setAttribute(QtCore.Qt.AA_X11InitThreads, True)

from PyQt5.QtQuick import QQuickView
from PyQt5.QtWidgets import QApplication, qApp, QWidget
from PyQt5.QtGui import QSurfaceFormat, QColor, QKeySequence, QCursor, QFont, QFontMetrics
from PyQt5.QtCore import QObject, Q_CLASSINFO, pyqtSlot, pyqtProperty, pyqtSignal, QTimer, QThread, QEvent
from PyQt5.QtDBus import QDBusAbstractAdaptor, QDBusConnection, QDBusConnectionInterface, QDBusMessage
import os
import sys
import json
import signal
import subprocess
from uuid import uuid4

import xcb
from xcb import xproto
from xcb.xproto import EventMask, GrabMode

from DBusInterfaces import MenuObjectInterface

SCREEN_WIDTH = 0
SCREEN_HEIGHT = 0

def getCursorPosition():
    return QCursor().pos()

def isInRect(x, y, xx, xy, xw, xh):
    return (xx <= x <= xx + xw) and (xy <= y <= xy + xh)

class MenuService(QObject):
    def __init__(self):
        super(MenuService, self).__init__()
        self.__dbusAdaptor = MenuServiceAdaptor(self)
        self._sessionBus = QDBusConnection.sessionBus()

        self.__menu = None

    def __checkToRestart(self):
        if self.__restart_flag:
            INJECTION.startNewService()
        self.__restart_flag = True
        del self.__timer

    def registerMenu(self):
        self.__restart_flag = False

        objPath = "/com/deepin/menu/%s" % str(uuid4()).replace("-", "_")
        objPathHolder= objPath.replace("/", "_")
        setattr(self, objPathHolder, MenuObject(self, objPath))
        self._sessionBus.registerObject(objPath, getattr(self, objPathHolder))
        return objPath

    def unregisterMenu(self, objPath):
        self.__restart_flag = True

        self._sessionBus.unregisterObject(objPath)
        setattr(self, objPath.replace("/", "_"), None)
        msg = QDBusMessage.createSignal(objPath, 'com.deepin.menu.Menu', 'MenuUnregistered')
        QDBusConnection.sessionBus().send(msg)

        self.__timer = QTimer()
        self.__timer.singleShot = True
        self.__timer.timeout.connect(self.__checkToRestart)
        self.__timer.start(5000)

    def showMenu(self, dbusObj, menuJsonContent):
        if self.__menu:
            self.__menu.destroySubs()
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

    def setItemText(self, id, value):
        msg = QDBusMessage.createSignal(self.objPath, 'com.deepin.menu.Menu', 'ItemTextSet')
        msg << id << value
        QDBusConnection.sessionBus().send(msg)

    def setItemActivity(self, id, value):
        msg = QDBusMessage.createSignal(self.objPath, 'com.deepin.menu.Menu', 'ItemActivitySet')
        msg << id << value
        QDBusConnection.sessionBus().send(msg)

    def setItemChecked(self, id, value):
        msg = QDBusMessage.createSignal(self.objPath, 'com.deepin.menu.Menu', 'ItemCheckedSet')
        msg << id << value
        QDBusConnection.sessionBus().send(msg)

class MenuObjectAdaptor(QDBusAbstractAdaptor):

    Q_CLASSINFO("D-Bus Interface", "com.deepin.menu.Menu")
    Q_CLASSINFO("D-Bus Introspection",
                '  <interface name="com.deepin.menu.Menu">\n'
                '    <method name="ShowMenu">\n'
                '      <arg direction="in" type="s" name="menuJsonContent"/>\n'
                '    </method>\n'
                '    <method name="SetItemActivity">\n'
                '      <arg direction="in" type="s" name="itemId"/>\n'
                '      <arg direction="in" type="b" name="isActive"/>\n'
                '    </method>\n'
                '    <method name="SetItemChecked">\n'
                '      <arg direction="in" type="s" name="itemId"/>\n'
                '      <arg direction="in" type="b" name="checked"/>\n'
                '    </method>\n'
                '    <method name="SetItemText">\n'
                '      <arg direction="in" type="s" name="itemId"/>\n'
                '      <arg direction="in" type="s" name="text"/>\n'
                '    </method>\n'
                '    <signal name="ItemInvoked">\n'
                '      <arg direction="out" type="s" name="itemId"/>\n'
                '      <arg direction="out" type="b" name="checked"/>\n'
                '    </signal>\n'
                '    <signal name="ItemTextSet">\n'
                '      <arg direction="out" type="s" name="itemId"/>\n'
                '      <arg direction="out" type="s" name="text"/>\n'
                '    </signal>\n'
                '    <signal name="ItemActivitySet">\n'
                '      <arg direction="out" type="s" name="itemId"/>\n'
                '      <arg direction="out" type="b" name="isActive"/>\n'
                '    </signal>\n'
                '    <signal name="ItemCheckedSet">\n'
                '      <arg direction="out" type="s" name="itemId"/>\n'
                '      <arg direction="out" type="b" name="checked"/>\n'
                '    </signal>\n'
                '    <signal name="MenuUnregistered">\n'
                '    </signal>\n'
                '  </interface>\n')

    ItemInvoked = pyqtSignal(str, bool)
    ItemTextSet = pyqtSignal(str, str)
    ItemActivitySet = pyqtSignal(str, bool)
    ItemCheckedSet = pyqtSignal(str, bool)
    MenuUnregistered = pyqtSignal()

    def __init__(self, parent):
        super(MenuObjectAdaptor, self).__init__(parent)

    @pyqtSlot(str)
    def ShowMenu(self, menuJsonContent):
        self.parent().showMenu(menuJsonContent)

    @pyqtSlot(str, str)
    def SetItemText(self, id, value):
        self.parent().setItemText(id, value)

    @pyqtSlot(str, bool)
    def SetItemActivity(self, id, value):
        self.parent().setItemActivity(id, value)

    @pyqtSlot(str, bool)
    def SetItemChecked(self, id, value):
        self.parent().setItemChecked(id, value)

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

    def startNewService(self):
        subprocess.Popen(["python", "-OO", os.path.join(os.path.dirname(__file__), "main.py")])

INJECTION = Injection()

class XGraber(QThread):
    def __init__(self, owner=None):
        super(QThread, self).__init__()
        self.owner = owner
        self._conn = xcb.connect()
        self.__pointer_grab_flag = False
        self.__keyboard_grab_flag = False

    @property
    def owner_wid(self):
        return self.owner.winId().__int__() if self.owner else None

    def grab_pointer(self):
        if not self.owner_wid or self.__pointer_grab_flag: return
        mask = EventMask.PointerMotion | EventMask.ButtonRelease | EventMask.ButtonPress
        while self._conn.core.GrabPointer(False, self.owner_wid, mask, GrabMode.Async, GrabMode.Async,
                                          0, 0,
                                          xproto.Time.CurrentTime).reply().status != 0:
            pass
        self.__pointer_grab_flag = True

    def ungrab_pointer(self):
        if not self.owner_wid or not self.__pointer_grab_flag: return
        self._conn.core.UngrabPointerChecked(xproto.Time.CurrentTime).check()
        self.__pointer_grab_flag = False

    def grab_keyboard(self):
        if not self.owner_wid or self.__keyboard_grab_flag: return
        while self._conn.core.GrabKeyboard(False, self.owner_wid, xproto.Time.CurrentTime,
                                           GrabMode.Async, GrabMode.Async).reply().status != 0:
            pass
        self.__keyboard_grab_flag = True

    def ungrab_keyboard(self):
        if not self.owner_wid or not self.__keyboard_grab_flag: return
        self._conn.core.UngrabKeyboardChecked(xproto.Time.CurrentTime).check()
        self.__keyboard_grab_flag = False

    def run(self):
        while True:
            e = self._conn.poll_for_event()
            if e == None :
                # self.yieldCurrentThread()
                self.usleep(400)
            if isinstance(e, xproto.MotionNotifyEvent):
                if self.owner and self.owner.inMenuArea(e.root_x, e.root_y):
                    self.ungrab_pointer()
                    self.ungrab_keyboard()
            elif isinstance(e, xproto.ButtonPressEvent):
                self.ungrab_pointer()
                self.ungrab_keyboard()
                self.owner.destroyWholeMenu()


class Menu(QQuickView):
    def __init__(self, dbusObj, menuJsonContent, parent=None):
        QQuickView.__init__(self)
        self.parent = parent
        self.subMenu = None

        self.setMenuJsonContent(menuJsonContent)
        self.setDBusObj(dbusObj)

        self.installEventFilter(self)
        qApp.focusWindowChanged.connect(self.focusWindowChangedSlot)

    def setDBusObj(self, dbusObj):
        self.dbusObj = dbusObj
        self.dbus_interface = MenuObjectInterface(self.dbusObj.objPath)
        self.dbus_interface.ItemTextSet.connect(self.updateItemText)
        self.dbus_interface.ItemActivitySet.connect(self.updateItemActivity)
        self.dbus_interface.ItemCheckedSet.connect(self.updateCheckableItem)

    def setMenuJsonContent(self, menuJsonContent):
        self.__menuJsonContent = menuJsonContent

    def focusWindowChangedSlot(self, window):
        if not self:
            return
        if window == None:
            self.destroyWholeMenu()

    def inMenuArea(self, x, y):
        if isInRect(x, y, self.x(), self.y(), self.width(), self.height()):
            return True
        elif self.subMenu and self.subMenu.inMenuArea(x, y):
            return True
        return False

    def eventFilter(self, obj, event):
        cursor_pos = getCursorPosition()
        if isinstance(obj, Menu) and event.type() == QEvent.Leave \
           and not self.ancestor.inMenuArea(cursor_pos.x(), cursor_pos.y()):
            self.grab_pointer()
            self.grab_keyboard()
        return QWidget.eventFilter(self, obj, event)

    def grab_pointer(self):
        xgraber.owner = self.ancestor
        xgraber.grab_pointer()

    def grab_keyboard(self):
        xgraber.owner = self.ancestor
        xgraber.grab_keyboard()

    @property
    def ancestor(self):
        if not self.parent:
            return self
        else: return self.parent.ancestor

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
    def notifyUpdateItemChecked(self, id, value):
        self.dbus_interface.setItemChecked(id, value)

    def updateCheckableItem(self, id, value):
        self.rootObject().updateCheckableItem(id, value)

    def updateItemActivity(self, id, value):
        self.rootObject().updateItemActivity(id, value)

    def updateItemText(self, id, value):
        self.rootObject().updateItemText(id, value)

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
        self.grab_pointer()
        self.grab_keyboard()
        
    def destroyForward(self):
        if self.subMenu:
            self.subMenu.destroyForward()
        self.destroy()
        
    @pyqtSlot()
    def destroyWholeMenu(self):
        menuService.unregisterMenu(self.dbusObj.objPath)
        self.ancestor.destroyForward()
        
    @pyqtSlot()
    def destroySubs(self):
        if self.subMenu: self.subMenu.destroyForward()
        self.requestActivate()

@pyqtSlot(str)
def serviceReplacedByOtherSlot(name):
    os._exit(0)

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

    xgraber = XGraber()
    xgraber.start()
    #xgraber.setPriority(QThread.LowestPriority)

    signal.signal(signal.SIGINT, signal.SIG_DFL)
    sys.exit(app.exec_())
