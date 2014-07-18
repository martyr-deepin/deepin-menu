#include <QGuiApplication>

#include "dbus_manager_adaptor.h"
#include "manager_object.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    ManagerObject manager;
    ManagerAdaptor manger(&manager);

    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerService("com.deepin.menu");
    connection.registerObject("/com/deepin/menu", &manager);

    return app.exec();
}
