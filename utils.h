#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <xcb/xcb.h>
#include <xcb/xproto.h>

class Utils : public QObject
{
    Q_OBJECT

public:
    static Utils* instance() {
        static Utils *utils = new Utils();
        return utils;
    }

private:
    Utils();
    Utils(const Utils &);
    Utils& operator=(const Utils &);

signals:
    void itemClicked(const QString &id, bool checked);
    void menuDisappeared();

public slots:
    void grabKeyboard(xcb_window_t);
    void grabPointer(xcb_window_t);
    void grabAll(xcb_window_t);

private:
    xcb_connection_t *conn;
    xcb_screen_t *screen;
};

#endif // UTILS_H
