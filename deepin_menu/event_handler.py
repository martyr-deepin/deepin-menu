#! /usr/bin/python
# -*- coding: utf-8 -*-

# Copyright (C) 2011 ~ 2013 Deepin, Inc.
#               2011 ~ 2013 Wang Yong
# 
# Author:     Wang Yong <lazycat.manatee@gmail.com>
# Maintainer: Wang Yong <lazycat.manatee@gmail.com>
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

from PyQt5.QtCore import QObject, pyqtSignal, pyqtSlot
from Xlib import X
from xutils import get_keyname

class EventHandler(QObject):
    press_esc = pyqtSignal()
    left_button_press = pyqtSignal(int, int, int)
    right_button_press = pyqtSignal(int, int, int)    
    
    def __init__(self):
        QObject.__init__(self)

    @pyqtSlot("QVariant")    
    def handle_event(self, event):
        if event.type == X.KeyPress:
            keyname = get_keyname(event)
            if keyname in ["Escape"]:
                self.press_esc.emit()
                
        elif event.type == X.ButtonPress:
            if event.detail == 1:
                self.left_button_press.emit(event.root_x, event.root_y, event.time)
                
            elif event.detail == 3:
                self.right_button_press.emit(event.root_x, event.root_y, event.time)
