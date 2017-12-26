/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QColor>
#include <QGuiApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtGlobal>
#include <QDebug>

#include "menu_object.h"
#include "ddesktopmenu.h"
#include "ddockmenu.h"

static DArrowRectangle::ArrowDirection DirectionFromString(QString direction) {
    if (direction == "top") {
        return DArrowRectangle::ArrowTop;
    } else if (direction == "left") {
        return DArrowRectangle::ArrowLeft;
    } else if (direction == "right") {
        return DArrowRectangle::ArrowRight;
    }

    return DArrowRectangle::ArrowBottom;
}

MenuObject::MenuObject():
    QObject(),
    m_dockMenu(nullptr),
    m_desktopMenu(nullptr),
    m_quitTimer(new QTimer(this))
{
    m_quitTimer->setInterval(60 * 1000);
    m_quitTimer->setSingleShot(true);

    connect(m_quitTimer, &QTimer::timeout, this, [=] {
        qApp->exit();
    });

    connect(this, &MenuObject::ItemInvoked, m_quitTimer, static_cast<void (QTimer::*)()>(&QTimer::start));
}

MenuObject::~MenuObject()
{
    if (m_dockMenu) m_dockMenu->destroyAll();
    if (m_desktopMenu) m_desktopMenu->deleteLater();

    emit MenuUnregistered();
}

void MenuObject::SetItemActivity(const QString &itemId, bool isActive)
{
    if (m_dockMenu) m_dockMenu->setItemActivity(itemId, isActive);
    if (m_desktopMenu) m_desktopMenu->setItemActivity(itemId, isActive);
}

void MenuObject::SetItemChecked(const QString &itemId, bool checked)
{
    if (m_dockMenu) m_dockMenu->setItemChecked(itemId, checked);
    if (m_desktopMenu) m_desktopMenu->setItemChecked(itemId, checked);
}

void MenuObject::SetItemText(const QString &itemId, const QString &text)
{
    if (m_dockMenu) m_dockMenu->setItemText(itemId, text);
    if (m_desktopMenu) m_desktopMenu->setItemText(itemId, text);
}

void MenuObject::ShowMenu(const QString &menuJsonContent)
{
    QByteArray bytes;
    bytes.append(menuJsonContent);
    QJsonDocument menuDocument = QJsonDocument::fromJson(bytes);
    QJsonObject jsonObj = menuDocument.object();

    int x = jsonObj["x"].toDouble();
    int y = jsonObj["y"].toDouble();
    QString direction = jsonObj["direction"].toString();

    if(jsonObj["isDockMenu"].toBool()) {
        m_dockMenu = new DDockMenu;
        m_dockMenu->installEventFilter(this);
        connect(m_dockMenu, &DDockMenu::destroyed, this, &MenuObject::menuDismissedSlot);
        connect(m_dockMenu, &DDockMenu::itemClicked, this, &MenuObject::ItemInvoked);
    } else {
        m_desktopMenu = new DDesktopMenu;
        m_desktopMenu->installEventFilter(this);
        connect(m_desktopMenu, &DDesktopMenu::aboutToHide, this, &MenuObject::menuDismissedSlot);
        connect(m_desktopMenu, &DDesktopMenu::itemClicked, this, &MenuObject::ItemInvoked);
    }

    bytes.clear();
    bytes.append(jsonObj["menuJsonContent"].toString());
    QJsonDocument menuContent = QJsonDocument::fromJson(bytes);
    QJsonObject menuContentObj = menuContent.object();

    if (m_dockMenu) {
        m_dockMenu->setArrowDirection(DirectionFromString(direction));
        m_dockMenu->setItems(menuContentObj["items"].toArray());
        m_dockMenu->show(x, y);
        m_dockMenu->grabFocus();
    } else if (m_desktopMenu) {
        m_desktopMenu->setItems(menuContentObj["items"].toArray());
        m_desktopMenu->popup(QPoint(x, y));
        m_desktopMenu->grabFocus();
    }
}

void MenuObject::menuDismissedSlot()
{
    if (m_dockMenu) {
        m_dockMenu->deleteLater();
        m_dockMenu = nullptr;
    } else if (m_desktopMenu) {
        m_desktopMenu->deleteLater();
        m_desktopMenu = nullptr;
    }

    this->deleteLater();
}

bool MenuObject::eventFilter(QObject *watched, QEvent *event)
{
    // NOTE(kirigaya): when menu hide, timed exit;
    if (watched == m_dockMenu || watched == m_desktopMenu) {
        if (event->type() == QEvent::Show)
            m_quitTimer->stop();
        if (event->type() == QEvent::Hide)
            m_quitTimer->start();
    }

    return false;
}
