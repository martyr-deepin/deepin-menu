/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
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

#ifndef MENU_OBJECT_H
#define MENU_OBJECT_H

#include <QObject>
#include <QPointer>

class DDockMenu;
class DDesktopMenu;
class MenuObject : public QObject
{
    Q_OBJECT
public:
    MenuObject();
    ~MenuObject();

signals:
    void ItemInvoked(const QString &itemId, bool checked);
    void MenuUnregistered();

public slots:
    void SetItemActivity(const QString &itemId, bool isActive);
    void SetItemChecked(const QString &itemId, bool checked);
    void SetItemText(const QString &itemId, const QString &text);

    void ShowMenu(const QString &menuJsonContent);

private slots:
    void menuDismissedSlot();

private:
    QPointer<DDockMenu> m_dockMenu;
    QPointer<DDesktopMenu> m_desktopMenu;
};

#endif // MENU_OBJECT_H
