#! /usr/bin/env python
# -*- coding: utf-8 -*-

from PyQt5.QtCore import pyqtSlot, pyqtSignal
from PyQt5.QtDBus import QDBusAbstractInterface, QDBusConnection

DBUS_SERVICE = "com.deepin.menu"
DBUS_PATH = "/com/deepin/menu"
DBUS_INTERFACE = "com.deepin.menu.Menu"

class MenuServiceInterface(QDBusAbstractInterface):
    
    ItemInvoked = pyqtSignal(int)

    def __init__(self, ):
        super(MenuServiceInterface, self).__init__(DBUS_SERVICE, DBUS_PATH, DBUS_INTERFACE, 
                                                   QDBusConnection.sessionBus(), None)

    def showMenu(self, x, y, content):
        self.call('ShowMenu', x, y, content)

@pyqtSlot(int)
def itemInvoked(itemId):
    print itemId
        
if __name__ == "__main__":
    import sys
    from PyQt5.QtCore import QCoreApplication
    
    app = QCoreApplication([])
    
    iface = MenuServiceInterface()
    iface.ItemInvoked.connect(itemInvoked)
    
    sys.exit(app.exec_())
