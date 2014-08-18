#include <QColor>
#include <QGuiApplication>

#include "utils.h"
#include "menu_object.h"
#include "manager_object.h"
#include "ddesktopmenu.h"

MenuObject::MenuObject(ManagerObject *manager):
    QObject()
{
    this->menu = NULL;
    this->manager = manager;

    connect(Utils::instance(), SIGNAL(itemClicked(QString,bool)), this, SIGNAL(ItemInvoked(QString,bool)));
    connect(Utils::instance(), SIGNAL(menuDisappeared()), manager, SLOT(UnregisterMenu()));
}

MenuObject::~MenuObject()
{
    if(menu) delete menu;
    menu = NULL;
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
    this->menu = new DDesktopMenu();
    this->menu->move(300, 300);
    this->menu->show();

//    Utils::instance()->grabKeyboard(menu->winId());
}

void MenuObject::destroyMenu()
{
    emit MenuUnregistered();
}
