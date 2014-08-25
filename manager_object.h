#ifndef MANAGER_OBJECT_H
#define MANAGER_OBJECT_H

#include <QObject>
#include <QString>
#include <QDBusObjectPath>

#include <dbus_menu_adaptor.h>
#include <menu_object.h>

class ManagerObject : public QObject
{
    Q_OBJECT
public:
    explicit ManagerObject(QObject *parent = 0);

signals:

public slots:
    QDBusObjectPath RegisterMenu();
    void UnregisterMenu();
    void UnregisterMenu(const QString &menuObjectPath);

private:
    MenuObject *menuObject;
    MenuAdaptor *menuAdaptor;
    QString menuObjectPath;

private slots:
    void menuObjectDestroiedSlot();
};

#endif // MANAGER_OBJECT_H
