#! /usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright (C) 2011 ~ 2014 Deepin, Inc.
#               2011 ~ 2014 Wang YaoHua
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

import logging  
import functools

SIMPLE = "_logger_simple_"
DETAIL = "_logger_detail_"
  
logger = logging.getLogger('mylogger')  
logger.setLevel(logging.DEBUG)  
  
ch = logging.StreamHandler()  
ch.setLevel(logging.DEBUG)  
  
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')  

ch.setFormatter(formatter)  
logger.addHandler(ch)  

def func_logger(fn, level=SIMPLE):
    def wrapper(*args, **kwds):
        if level == SIMPLE:
            logger.debug(fn.func_name)
        else:
            logger.debug(fn.func_name)
            logger.debug(fn.func_dict)
        return fn(*args, **kwds)
    functools.update_wrapper(wrapper, fn)
    return wrapper
