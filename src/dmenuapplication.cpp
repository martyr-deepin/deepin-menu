/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "dmenuapplication.h"

DMenuApplication::DMenuApplication(int &argc, char *argv[]) :
    DApplication(argc, argv)
{
    setTheme("light");
}

void DMenuApplication::quitApplication(const QString &)
{
    this->quit();
}
