#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QStringList>
#include <xcb/xcb.h>
#include <xcb/xproto.h>

class QX11Info;
class QString;
class QPoint;
class QRect;

class Utils : public QObject
{
    Q_OBJECT

public:
    static bool menuItemCheckableFromId(QString id) {
        return id.split(':').count() == 3;
    }

    static QString toMenuItemText(QString text);

    static void grabKeyboard(xcb_window_t);
    static void grabPointer(xcb_window_t);

    static bool pointInRect(QPoint, QRect);

private:
    Utils();
    Utils(const Utils &);
    Utils& operator=(const Utils &);
};

#endif // UTILS_H
