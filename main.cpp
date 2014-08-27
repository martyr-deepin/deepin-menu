#include <QDBusConnectionInterface>

#include "dbus_manager_adaptor.h"
#include "manager_object.h"
#include "dmenuapplication.h"

#define MENU_SERVICE_NAME "com.deepin.menu"
#define MENU_SERVICE_PATH "/com/deepin/menu"

int main(int argc, char *argv[])
{
    DMenuApplication app(argc, argv);

    ManagerObject managerObject;
    ManagerAdaptor manager(&managerObject);

    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.interface()->registerService(MENU_SERVICE_NAME,
                                            QDBusConnectionInterface::ReplaceExistingService,
                                            QDBusConnectionInterface::AllowReplacement);
    connection.registerObject(MENU_SERVICE_PATH, &managerObject);
    DMenuApplication::connect(connection.interface(), SIGNAL(serviceUnregistered(QString)),
                              &app, SLOT(quitApplication(QString)));

    return app.exec();
}
