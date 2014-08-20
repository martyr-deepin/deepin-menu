#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QStringList>
#include <xcb/xcb.h>
#include <xcb/xproto.h>

#define DISPLAY_SERVICE "com.deepin.daemon.Display"
#define DISPLAY_PATH "/com/deepin/daemon/Display"
#define PROPERTIES_INTERFACE "org.freedesktop.DBus.Properties"
#define DISPLAY_INTERFACE "com.deepin.daemon.Display"
#define MONITOR_INTERFACE "com.deepin.daemon.Display.Monitor"

class QX11Info;
class QString;
class QPoint;
class QRect;

namespace Utils {

bool menuItemCheckableFromId(QString id);

void grabKeyboard(xcb_window_t);
void grabPointer(xcb_window_t);

bool pointInRect(QPoint, QRect);
QRect currentMonitorRect(int, int);

}

#endif // UTILS_H
