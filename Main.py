#! /usr/bin/python
# -*- coding: utf-8 -*-

from PyQt5.QtWidgets import QApplication
from PyQt5.QtQuick import QQuickView
from PyQt5.QtGui import QSurfaceFormat, QColor, QCursor
from PyQt5 import QtCore, QtQuick
from PyQt5.QtCore import QSize, pyqtSlot
import os
import sys
import signal

items = [{"icon":"/usr/share/icons/Deepin/apps/16/preferences-display.png",
          "label":"Display",
          "subMenu": {"icon":"/usr/share/icons/Deepin/apps/16/preferences-display.png"}},
         {"icon":"/usr/share/icons/Deepin/apps/16/preferences-display.png",
          "label":"Display",
          "subMenu": None}]

SCREEN_WIDTH = 0
SCREEN_HEIGHT = 0

def getCursorPosition():
    return QCursor().pos()

def getWidthFromLabelLength(length):
    return length * 10

def getHeightFromItemsNum(num):
    return num * 10

class Menu(QQuickView):

    def __init__(self, menuItems=None):
        QQuickView.__init__(self)

        self.menuItems = menuItems

        qml_context = self.rootContext()
        qml_context.setContextProperty("_menu_view", self)

        self.setResizeMode(QtQuick.QQuickView.SizeRootObjectToView)
        self.setMinimumSize(QSize(70, 50))

        surface_format = QSurfaceFormat()
        surface_format.setAlphaBufferSize(8)
        self.setFormat(surface_format)

        self.setColor(QColor(0, 0, 0, 0))
        self.setFlags(QtCore.Qt.FramelessWindowHint)
        self.setSource(QtCore.QUrl.fromLocalFile(os.path.join(os.path.dirname(__file__), 'Menu.qml')))
        
    @pyqtSlot(result=int)
    def getItemsNum(self):
        return len(self.menuItems)

    @pyqtSlot(result=list)
    def getItem(self, index):
        return self.menuItems[index]

    def maxWidth(self):
        maxLabel = max([len(menuItem["label"]) for menuItem in self.menuItems])
        return getWidthFromLabelLength(maxLabel)
    
    def maxHeight(self):
        return getHeightFromItemsNum(self.getItemsNum())

    @pyqtSlot(int)
    def showSubMenu(self, itemId):
        # sub = Menu([self.menuItems[itemId],])
        # if self.x() + self.width() + sub.maxWidth() > SCREEN_WIDTH:
        #     sub.setX(self.x()  - sub.maxWidth())
        # else:
        #     sub.setX(self.x() + self.width())
        sub = Menu(items)
        sub.setX(300)
        sub.setY(300)
            
        sub.show()

if __name__ == "__main__":
    app = QApplication(sys.argv)
    
    # retrieve geometry of the screen
    desktopWidget = app.desktop()
    screenGeometry = desktopWidget.screenGeometry()
    SCREEN_WIDTH = screenGeometry.width()
    SCREEN_HEIGHT = screenGeometry.height()

    view = Menu(items)
    view.setX(100)
    view.setY(100)
    view.show()

    signal.signal(signal.SIGINT, signal.SIG_DFL)
    sys.exit(app.exec_())
