#include <QColor>
#include <QGuiApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtGlobal>
#include <QDebug>

#include "utils.h"
#include "menu_object.h"
#include "manager_object.h"
#include "ddesktopmenu.h"
#include "ddockmenu.h"

MenuObject::MenuObject(ManagerObject *manager):
    QObject(manager)
{
    this->menu = NULL;
}

void MenuObject::SetItemActivity(const QString &itemId, bool isActive)
{

}

void MenuObject::SetItemChecked(const QString &itemId, bool checked)
{

}

void MenuObject::SetItemText(const QString &itemId, const QString &text)
{

}

void MenuObject::ShowMenu(const QString &menuJsonContent)
{
    Q_ASSERT(this->menu == NULL);

    QByteArray bytes;
    bytes.append(menuJsonContent);
    QJsonDocument menuDocument = QJsonDocument::fromJson(bytes);
    QJsonObject jsonObj = menuDocument.object();

    if(jsonObj["isDockMenu"].toBool()) {
        this->menu = new DDockMenu();
    } else {
        this->menu = new DDesktopMenu();
    }

    connect(this->menu, SIGNAL(destroyed()), this, SLOT(menuDestroiedSlot()));
    connect(this->menu, SIGNAL(itemClicked(QString,bool)), this, SIGNAL(ItemInvoked(QString,bool)));

    bytes.clear();
    bytes.append(jsonObj["menuJsonContent"].toString());
    QJsonDocument menuContent = QJsonDocument::fromJson(bytes);
    QJsonObject menuContentObj = menuContent.object();

    this->menu->setContent(menuContentObj["items"].toArray());
    this->menu->setPosition(jsonObj["x"].toInt(), jsonObj["y"].toInt());
    this->menu->show();
    this->menu->grabFocus();
}

void MenuObject::destroyMenu()
{
    if(this->menu) {
        this->menu->deleteLater();
    }

    emit MenuUnregistered();
}

void MenuObject::menuDestroiedSlot()
{
    this->menu = NULL;
}
