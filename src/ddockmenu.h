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
#include "dabstractmenu.h"
#include <DWindowManagerHelper>
#include <com_deepin_api_xmousearea.h>

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

    void grabFocus() Q_DECL_OVERRIDE;
    void releaseFocus() Q_DECL_OVERRIDE;

    void destroyAll();

signals:
    void itemClicked(const QString &id, bool checked);

private slots:
    void onButtonPress(int in0, int in1, int in2, const QString &in3);
    void onCursorMove(int in0, int in1, const QString &in2);
    void onKeyPress(const QString &in0, int in1, int in2, const QString &in3);
    void onWMCompositeChanged();

private:
    DDockMenu *getRootMenu();
    DDockMenu *menuUnderPoint(const QPoint point);
    void showSubMenu(int x, int y, const QJsonObject &obj );

    bool event(QEvent *event) Q_DECL_OVERRIDE;

private:
    friend class DMenuContent;
    DMenuContent *m_menuContent;

    ItemStyle normalStyle;
    ItemStyle hoverStyle;
    ItemStyle inactiveStyle;

    QString m_mouseAreaKey;
    com::deepin::api::XMouseArea *m_mouseAreaInter;

    DWindowManagerHelper *m_wmHelper;
};

#endif // DDOCKMENU_H
