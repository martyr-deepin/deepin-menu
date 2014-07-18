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
    ~ManagerObject();

signals:

public slots:
    QDBusObjectPath RegisterMenu();
    void UnregisterMenu(const QString &menuObjectPath);

private:
    MenuObject *menuObject;
    MenuAdaptor *menuAdaptor;
    QString menuObjectPath;
};

#endif // MANAGER_OBJECT_H
