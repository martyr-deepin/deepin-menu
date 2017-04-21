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

#include <QMenu>
#include "dabstractmenu.h"

#include <com_deepin_api_xmousearea.h>

using namespace com::deepin::api;

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

    QString m_key;
    XMouseArea *m_mouseArea;

    QList<QMenu*> m_ownMenus;
};

#endif // DDESKTOPMENU_H
