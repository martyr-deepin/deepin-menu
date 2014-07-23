#include "utils.h"

#include <xcb/xcb.h>
#include <xcb/xproto.h>

#include <QtX11Extras/QX11Info>
#include <QDebug>

Utils::Utils()
{
    conn = QX11Info::connection();
}

void Utils::grabKeyboard(xcb_window_t window)
{
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

void Utils::grabPointer(xcb_window_t window) {
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
