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

#ifndef DDOCKMENU_H
#define DDOCKMENU_H

#include "dabstractmenu.h"
#include <dregionmonitor.h>
#include <darrowrectangle.h>
#include <DWindowManagerHelper>

DWIDGET_USE_NAMESPACE

struct ItemStyle {
    QColor itemBackgroundColor;
    QColor itemTextColor;
    QColor itemShortcutColor;
    QString checkmarkIcon;
    QString subMenuIndicatorIcon;
};

class DMenuContent;
class DDockMenu : public DArrowRectangle, public DAbstractMenu
{
    Q_OBJECT
public:
    explicit DDockMenu(DDockMenu *parent = 0);
    ~DDockMenu();

    void setItems(QJsonArray items) Q_DECL_OVERRIDE;

    void releaseFocus() Q_DECL_OVERRIDE;

    void destroyAll();

signals:
    void itemClicked(const QString &id, bool checked);

private slots:
    void onWMCompositeChanged();

private:
    DDockMenu *getRootMenu();
    DDockMenu *menuUnderPoint(const QPoint point);
    void showSubMenu(int x, int y, const QJsonObject &obj );

protected:
    bool event(QEvent *event) Q_DECL_OVERRIDE;
    void showEvent(QShowEvent *e) Q_DECL_OVERRIDE;
    void hideEvent(QHideEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *event);

private:
    friend class DMenuContent;
    DMenuContent *m_menuContent;

    ItemStyle normalStyle;
    ItemStyle hoverStyle;
    ItemStyle inactiveStyle;
    DRegionMonitor *m_monitor;
    DWindowManagerHelper *m_wmHelper;
};

#endif // DDOCKMENU_H
