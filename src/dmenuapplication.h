/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DMENUAPPLICATION_H
#define DMENUAPPLICATION_H

#include <DApplication>

DWIDGET_USE_NAMESPACE

class DMenuApplication : public DApplication
{
    Q_OBJECT
public:
    explicit DMenuApplication(int &, char**);

public slots:
    void quitApplication(const QString&);
};

#endif // DMENUAPPLICATION_H
