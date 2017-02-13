/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DDESKTOPMENU_H
#define DDESKTOPMENU_H

#include "src/dmenubase.h"

class QJsonObject;
class DDesktopMenu : public DMenuBase
{
    Q_OBJECT
public:
    DDesktopMenu(DDesktopMenu *parent=0);

    virtual void setPosition(int, int);

protected:
    virtual void paintEvent(QPaintEvent * event);

private:
    virtual void showSubMenu(int, int, QJsonObject);
};

#endif // DDESKTOPMENU_H
