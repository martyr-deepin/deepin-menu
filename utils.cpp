/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include "utils.h"

#include <xcb/xcb.h>
#include <xcb/xproto.h>

#include <QtX11Extras/QX11Info>
#include <QPoint>
#include <QRect>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusObjectPath>
#include <QDBusVariant>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

static QRect s_currentMonitorRect(0, 0, 0, 0);

namespace Utils {

bool menuItemCheckableFromId(QString id)
{
    return id.split(':').count() == 3;
}

void grabKeyboard(xcb_window_t window)
{
    xcb_connection_t *conn = QX11Info::connection();
    xcb_grab_keyboard_cookie_t cookie = xcb_grab_keyboard(conn,
                                                          true,                /* report events */
                                                          window,
                                                          XCB_CURRENT_TIME,
                                                          XCB_GRAB_MODE_ASYNC, /* process events as normal, do not require sync */
                                                          XCB_GRAB_MODE_ASYNC
                                                          );

    xcb_grab_keyboard_reply_t *reply;
    if ((reply = xcb_grab_keyboard_reply(conn, cookie, NULL))) {
        if (reply->status == XCB_GRAB_STATUS_SUCCESS)
            qDebug() << "successfully grabbed the keyboard\n";

        free(reply);
    }
}

void grabPointer(xcb_window_t window) {
    xcb_connection_t *conn = QX11Info::connection();
    xcb_grab_pointer_cookie_t cookie = xcb_grab_pointer(conn,
                                                        true,               /* get all pointer events specified by the following mask */
                                                        window,
                                                        XCB_NONE,            /* which events to let through */
                                                        XCB_GRAB_MODE_ASYNC, /* pointer events should continue as normal */
                                                        XCB_GRAB_MODE_ASYNC, /* keyboard mode */
                                                        XCB_NONE,            /* confine_to = in which window should the cursor stay */
                                                        XCB_NONE,              /* we change the cursor to whatever the user wanted */
                                                        XCB_CURRENT_TIME
                                                        );

    xcb_grab_pointer_reply_t *reply;
    if ((reply = xcb_grab_pointer_reply(conn, cookie, NULL))) {
        if (reply->status == XCB_GRAB_STATUS_SUCCESS)
            qDebug() << "successfully grabbed the pointer\n";
        free(reply);
    }
}

bool pointInRect(QPoint point, QRect rect)
{
    return rect.x() <= point.x() &&
            point.x() <= rect.x() + rect.width() &&
            rect.y() <= point.y() &&
            point.y() <= rect.y() + rect.height();

}

// NOTE: this function is deprecated because of speed problem,
// use the function which is at the bottom of this file instead.
QRect _currentMonitorRect(int x, int y)
{
    qDebug() << "currentMonitorRect" << x << y;

    if (pointInRect(QPoint(x, y), s_currentMonitorRect)) return s_currentMonitorRect;

    QDBusInterface displayIface(DISPLAY_SERVICE,
                                DISPLAY_PATH,
                                PROPERTIES_INTERFACE,
                                QDBusConnection::sessionBus());

    if (displayIface.isValid()) {
        QDBusReply<QDBusVariant> reply = displayIface.call("Get", DISPLAY_INTERFACE, "Monitors");
        if (reply.isValid()) {
            qDebug() << reply.value().variant();
            QDBusArgument argument = reply.value().variant().value<QDBusArgument>();

            argument.beginArray();
            while (!argument.atEnd()) {
                QDBusObjectPath path;
                argument >> path;
                qDebug() << path.path();

                QDBusInterface monitorIface(DISPLAY_SERVICE,
                                            path.path(),
                                            PROPERTIES_INTERFACE,
                                            QDBusConnection::sessionBus());

                if (displayIface.isValid()) {
                    QRect _rect(0, 0, 0, 0);
                    QDBusReply<QDBusVariant> reply = monitorIface.call("Get", MONITOR_INTERFACE, "X");
                    if (reply.isValid()) {
                        _rect.setX(reply.value().variant().value<uint>());
                    }
                    reply = monitorIface.call("Get", MONITOR_INTERFACE, "Y");
                    if (reply.isValid()) {
                        _rect.setY(reply.value().variant().value<uint>());
                    }
                    reply = monitorIface.call("Get", MONITOR_INTERFACE, "Width");
                    if (reply.isValid()) {
                        _rect.setWidth(reply.value().variant().value<uint>());
                    }
                    reply = monitorIface.call("Get", MONITOR_INTERFACE, "Height");
                    if (reply.isValid()) {
                        _rect.setHeight(reply.value().variant().value<uint>());
                    }
                    if (pointInRect(QPoint(x, y), _rect)) s_currentMonitorRect = _rect;
                }
            }
            argument.endArray();
        }
    }

    return s_currentMonitorRect;
}

QRect currentMonitorRect(int x, int y)
{
    QDesktopWidget *desktop = QApplication::desktop();

    return desktop->screenGeometry(desktop->screenNumber(QPoint(x, y)));
}

}
