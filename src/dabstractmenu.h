/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     Hualet Wang <mr.asianwang@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DABSTRACTMENU_H
#define DABSTRACTMENU_H

#include <QObject>

class DAbstractMenu
{
public:
    explicit DAbstractMenu();

public:
    virtual void releaseFocus();

    virtual void setItems(QJsonArray items);

    virtual void setItemActivity(const QString &itemId, bool isActive);
    virtual void setItemChecked(const QString &itemId, bool checked);
    virtual void setItemText(const QString &itemId, const QString &text);
};

#endif // DABSTRACTMENU_H
