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

#include <com_deepin_api_xmousearea.h>

using namespace com::deepin::api;

class DDesktopMenu : public QMenu
{
    Q_OBJECT
public:
    explicit DDesktopMenu();
    ~DDesktopMenu();

    void setContent(QJsonArray items);
    void grabFocus();

    void setItemActivity(const QString &itemId, bool isActive);
    void setItemChecked(const QString &itemId, bool checked);
    void setItemText(const QString &itemId, const QString &text);

signals:
    void itemClicked(const QString &id, bool checked);

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

private:
    QAction *action(const QString &id);
    void addActionFromJson(QMenu *menu, const QJsonArray &items);

    QString m_key;
    XMouseArea *m_mouseArea;
};

#endif // DDESKTOPMENU_H
