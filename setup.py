#! /usr/bin/env python
# -*- coding: utf-8 -*-

# Copyright (C) 2015 Deepin Technology Co., Ltd.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.

from setuptools import setup
import os

def list_files(target_dir, install_dir):
    '''List files for option `data_files`.'''
    results = []
    for root, dirs, files in os.walk(target_dir):
        for filepath in files:
            data_dir = os.path.dirname(os.path.join(root, filepath))
            data_file = os.path.join(root, filepath)
            results.append((data_dir, [data_file]))
            # print results
    return results                

setup(name='deepin_menu',
      version='0.1',
      description='Menu Service for Linux Deepin.',
      long_description ="""Menu Service for Linux Deepin.""",
      author='Linux Deepin Team',
      author_email='wangyaohua@linuxdeepin.com',
      license='GPL-3',
      url="https://github.com/linuxdeepin/deepin-menu",
      download_url="git://github.com/linuxdeepin/deepin-menu.git",
      platforms = ['Linux'],
      packages = ['deepin_menu',],
      data_files = list_files("deepin_menu","deepin_menu"))

