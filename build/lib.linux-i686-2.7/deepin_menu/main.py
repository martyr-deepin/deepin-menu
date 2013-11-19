#! /usr/bin/python
# -*- coding: utf-8 -*-

from PyQt5 import QtCore, QtQuick
from PyQt5.QtWidgets import QApplication, qApp
from PyQt5.QtQuick import QQuickView
from PyQt5.QtGui import QSurfaceFormat, QColor, QCursor, QFont, QFontMetrics
from PyQt5.QtCore import QSize, QObject, Q_CLASSINFO, pyqtSlot, pyqtProperty, pyqtSignal
from PyQt5.QtDBus import QDBusAbstractAdaptor, QDBusConnection, QDBusMessage
import os
import sys
import signal

SCREEN_WIDTH = 0
SCREEN_HEIGHT = 0

def getCursorPosition():
    return QCursor().pos()

class MenuService(QObject):
    def __init__(self):
        super(MenuService, self).__init__()
        self.__dbusAdaptor = MenuServiceAdaptor(self)

        self.__view = None
        self.__injection = None

    def showMenu(self, x, y, content):
        self.__view = Menu(x, y, content, True)
        self.__injection = Injection()

        show_menu(self.__view, self.__injection)

    def showDockMenu(self, x, y, content):
        self.__view = Menu(x, y, content, False)
        self.__injection = Injection()

        show_menu(self.__view, self.__injection)

class MenuServiceAdaptor(QDBusAbstractAdaptor):
    """DBus service for creating beautiful menus."""

    Q_CLASSINFO("D-Bus Interface", "com.deepin.menu.Menu")
    Q_CLASSINFO("D-Bus Introspection",
                '  <interface name="com.deepin.menu.Menu">\n'
                '    <method name="ShowMenu">\n'
                '      <arg direction="in" type="i" name="x"/>\n'
                '      <arg direction="in" type="i" name="y"/>\n'
                '      <arg direction="in" type="s" name="content"/>\n'
                '    </method>\n'
                '    <method name="ShowDockMenu">\n'
                '      <arg direction="in" type="i" name="x"/>\n'
                '      <arg direction="in" type="i" name="y"/>\n'
                '      <arg direction="in" type="s" name="content"/>\n'
                '    </method>\n'
                '    <signal name="ItemInvoked">\n'
                '      <arg direction="out" type="s" name="itemId"/>\n'
                '    </signal>\n'
                '  </interface>\n')
    
    ItemInvoked = pyqtSignal(str)

    def __init__(self, parent):
        super(MenuServiceAdaptor, self).__init__(parent)

    @pyqtSlot(int, int, str)
    def ShowMenu(self, x, y, content):
        self.parent().showMenu(x, y, content)
        
    @pyqtSlot(int, int, str)
    def ShowDockMenu(self, x, y, content):
        self.parent().showDockMenu(x, y, content)
        
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
        font.setPixelSize(12)
        fm = QFontMetrics(font)
        return fm.width(string)

    @pyqtSlot(str,int,result=int)
    def getStringHeight(self, string, pixelSize):
        font = QFont()
        font.setPixelSize(12)
        fm = QFontMetrics(font)
        fm.width(string)
        return fm.height()

    # @pyqtSlot(int,int,int)
    # def postMouseEvent(self, x, y, button):
    #     print x, y, button
    #     mouseReleaseEvent = QMouseEvent(
    #         QEvent.MouseButtonPress,
    #         QPointF(x, y),
    #         QtCore.Qt.LeftButton,
    #         QtCore.Qt.LeftButton,
    #         QtCore.Qt.NoModifier,
    #         )
    #     QApplication.postEvent(self, mouseReleaseEvent)

class Menu(QQuickView):

    def __init__(self, x, y, menuJsonContent, withCorner):
        QQuickView.__init__(self)

        self.__x = x
        self.__y = y
        self.__menuJsonContent = menuJsonContent
        self.__withCorner = withCorner

    @pyqtProperty(int)
    def x(self):
        return self.__x

    @pyqtProperty(int)
    def y(self):
        return self.__y

    @pyqtProperty(bool)
    def withCorner(self):
        return self.__withCorner

    @pyqtProperty(str)
    def menuJsonContent(self):
        return self.__menuJsonContent
    
    @pyqtSlot(str)
    def invokeItem(self, id):
        msg = QDBusMessage.createSignal('/com/deepin/menu', 'com.deepin.menu.Menu', 'ItemInvoked')
        msg << id
        QDBusConnection.sessionBus().send(msg)
        
def show_menu(view, injection):
    qml_context = view.rootContext()
    # qml_context.setContextProperty("_menu_items", )
    qml_context.setContextProperty("_menu_view", view)
    qml_context.setContextProperty("_application", qApp)
    qml_context.setContextProperty("_injection", injection)

    view.setResizeMode(QtQuick.QQuickView.SizeRootObjectToView)
    view.setMinimumSize(QSize(70, 50))

    surface_format = QSurfaceFormat()
    surface_format.setAlphaBufferSize(8)
    view.setFormat(surface_format)

    view.setColor(QColor(0, 0, 0, 0))
    view.setFlags(QtCore.Qt.FramelessWindowHint)
    view.setSource(QtCore.QUrl.fromLocalFile(os.path.join(os.path.dirname(__file__), 'FullScreenMenu.qml')))

    view.show()

if __name__ == "__main__":
    app = QApplication(sys.argv)

    # app = QCoreApplication([])
    bus = QDBusConnection.sessionBus()
    menuService = MenuService()
    bus.registerObject('/com/deepin/menu', menuService)
    bus.registerService('com.deepin.menu')

    # retrieve the geometry of the screen
    desktopWidget = app.desktop()
    screenGeometry = desktopWidget.screenGeometry()
    SCREEN_WIDTH = screenGeometry.width()
    SCREEN_HEIGHT = screenGeometry.height()
    
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    sys.exit(app.exec_())
