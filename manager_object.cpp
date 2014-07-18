#include <QDBusObjectPath>
#include <QUuid>
#include <QDebug>

#include "dbus_menu_adaptor.h"
#include "manager_object.h"

ManagerObject::ManagerObject(QObject *parent) :
    QObject(parent)
{
}

ManagerObject::~ManagerObject()
{
    if(menuObject) delete menuObject;
    if(menuAdaptor) delete menuAdaptor;
}

QDBusObjectPath ManagerObject::RegisterMenu()
{
    QString uuid = QUuid::createUuid().toString();
    uuid = uuid.replace("{", "");
    uuid = uuid.replace("}", "");
    uuid = uuid.replace("-", "_");
    menuObjectPath = "/com/deepin/menu/" + uuid;

    if(menuObject) delete menuObject;
    if(menuAdaptor) delete menuAdaptor;
    menuObject = new MenuObject();
    menuAdaptor = new MenuAdaptor(menuObject);

    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerService("com.deepin.menu");
    connection.registerObject(menuObjectPath, menuObject);

    QDBusObjectPath result(menuObjectPath);
    return result;
}

void ManagerObject::UnregisterMenu(const QString &menuObjectPath)
{
    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.unregisterObject(menuObjectPath);
}
