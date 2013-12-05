#! /usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright (C) 2011 ~ 2013 Wang YaoHua
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

class CycleNumber(object):
    def __init__(self, end, start=0):
        self.start = start        
        self.end = end
        self._cursor = 0
        
        self.__value_dict = range(self.start, self.end)
        print self.__value_dict

    def next(self, step=0):
        self._cursor = (self._cursor + step) % (self.end - self.start)
        result = self.__value_dict[self._cursor]
        self._cursor += 1
        return result

    
if __name__ == "__main__":
    num = CycleNumber(10)
    print num.next()
    print num.next()
    print num.next(1)
    print num.next(2)
    print num.next(10)
