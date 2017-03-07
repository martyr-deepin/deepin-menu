/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DDOCKMENU_H
#define DDOCKMENU_H

#include <darrowrectangle.h>

DWIDGET_USE_NAMESPACE

struct ItemStyle {
    QColor itemBackgroundColor;
    QColor itemTextColor;
    QColor itemShortcutColor;
    QString checkmarkIcon;
    QString subMenuIndicatorIcon;
};

class DMenuContent;
class DDockMenu : public DArrowRectangle
{
    Q_OBJECT
public:
    explicit DDockMenu(DDockMenu *parent = 0);

    void setItemActivity(const QString &itemId, bool isActive);
    void setItemChecked(const QString &itemId, bool checked);
    void setItemText(const QString &itemId, const QString &text);

    void setItems(QJsonArray items);

    DDockMenu *getRootMenu();
    void showSubMenu(int x, int y, const QJsonObject &obj );

    DDockMenu *menuUnderPoint(const QPoint point);

    void grabFocus();
    void releaseFocus();

    void destroyAll();

    ItemStyle normalStyle;
    ItemStyle hoverStyle;
    ItemStyle inactiveStyle;

signals:
    void itemClicked(const QString &itemId, bool checked);

private:
    DMenuContent *m_menuContent;
};

#endif // DDOCKMENU_H
