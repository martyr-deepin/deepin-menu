#include <QDBusObjectPath>
#include <QUuid>
#include <QDebug>

#include "dbus_menu_adaptor.h"
#include "manager_object.h"

ManagerObject::ManagerObject(QObject *parent) :
    QObject(parent)
{
    menuObjectPath = "";
    menuObject = NULL;
}

QDBusObjectPath ManagerObject::RegisterMenu()
{
    this->UnregisterMenu();

    QString uuid = QUuid::createUuid().toString();
    uuid = uuid.replace("{", "");
    uuid = uuid.replace("}", "");
    uuid = uuid.replace("-", "_");
    menuObjectPath = "/com/deepin/menu/" + uuid;

    menuObject = new MenuObject(this);
    menuAdaptor = new MenuAdaptor(menuObject);

    QDBusConnection connection = QDBusConnection::sessionBus();
    connection.registerObject(menuObjectPath, menuObject);

    QDBusObjectPath result(menuObjectPath);
    return result;
}

void ManagerObject::UnregisterMenu()
{
    if (menuObjectPath != "") {
        this->UnregisterMenu(menuObjectPath);
    }
}

void ManagerObject::UnregisterMenu(const QString &menuObjectPath)
{
    if (menuObject) {
        menuObject->destroyMenu();
        menuObject->deleteLater();
        menuObject = NULL;

        QDBusConnection connection = QDBusConnection::sessionBus();
        connection.unregisterObject(menuObjectPath);
    }
}
