/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
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
#include "dabstractmenu.h"

#include <dregionmonitor.h>

DWIDGET_USE_NAMESPACE
class DMenuContent;
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
    void grabFocus() Q_DECL_OVERRIDE;

signals:
    void itemClicked(const QString &id, bool checked);

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    QAction *action(const QString &id);
    void addActionFromJson(QMenu *menu, const QJsonArray &items);
    bool containsPoint(const QPoint &point) const;

    DRegionMonitor *m_mouseArea;

    QList<QMenu*> m_ownMenus;
private:
    friend class DMenuContent;
    DMenuContent *m_menuContent;
};

#endif // DDESKTOPMENU_H
