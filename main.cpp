#include <QApplication>

#include "dbus_manager_adaptor.h"
#include "manager_object.h"

#include "ddockmenu.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ManagerObject manager;
    ManagerAdaptor manger(&manager);

    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerService("com.deepin.menu");
    connection.registerObject("/com/deepin/menu", &manager);

    DDockMenu menu;
    menu.show();

    return a.exec();
}
