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

#ifndef DDESKTOPMENU_H
#define DDESKTOPMENU_H

#include <QMenu>
#include <dregionmonitor.h>

#include "dabstractmenu.h"

DWIDGET_USE_NAMESPACE

class DDesktopMenu : public QMenu, public DAbstractMenu
{
    Q_OBJECT
public:
    explicit DDesktopMenu();
    ~DDesktopMenu();

    void setItems(QJsonArray items) Q_DECL_OVERRIDE;

    void setItemActivity(const QString &itemId, bool isActive) Q_DECL_OVERRIDE;
    void setItemChecked(const QString &itemId, bool checked) Q_DECL_OVERRIDE;
    void setItemText(const QString &itemId, const QString &text) Q_DECL_OVERRIDE;

    void showMenu(const QPoint pos, bool isScaled);

signals:
    void itemClicked(const QString &id, bool checked);

protected:
    void showEvent(QShowEvent *e) Q_DECL_OVERRIDE;
    void hideEvent(QHideEvent *e) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    QAction *action(const QString &id);
    void addActionFromJson(QMenu *menu, const QJsonArray &items);
    DRegionMonitor *m_monitor;
    QList<QMenu*> m_ownMenus;
};

#endif // DDESKTOPMENU_H
