/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef MENU_OBJECT_H
#define MENU_OBJECT_H

#include <QObject>
#include <QTimer>

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

protected:
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;

private:
    DDockMenu *m_dockMenu;
    DDesktopMenu *m_desktopMenu;
    QTimer *m_quitTimer;
};

#endif // MENU_OBJECT_H
