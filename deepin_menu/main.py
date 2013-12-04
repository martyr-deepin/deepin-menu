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

from PyQt5.QtCore import QCoreApplication
QCoreApplication.setAttribute(10, True)

import sys
import signal
from PyQt5.QtWidgets import QApplication
from PyQt5.QtDBus import QDBusConnection

from DBusBackend import MenuService


app = QApplication([])

bus = QDBusConnection.sessionBus()
menuService = MenuService()
bus.registerService('com.deepin.menu')
bus.registerObject('/com/deepin/menu', menuService)

signal.signal(signal.SIGINT, signal.SIG_DFL)
sys.exit(app.exec_())
