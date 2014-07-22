#include <QDebug>
#include <QQuickView>
#include <QQuickItem>
#include <QQmlEngine>
#include <QQmlContext>
#include <QColor>
#include <QDBusConnection>
#include <QGuiApplication>

#include "utils.h"
#include "menu_object.h"
#include "manager_object.h"

MenuObject::MenuObject(ManagerObject *manager):
    QObject()
{
    this->manager = manager;
    this->menu = new QQuickView();

    connect(Utils::instance(), SIGNAL(itemClicked(QString,bool)), this, SIGNAL(ItemInvoked(QString,bool)));
    connect(Utils::instance(), SIGNAL(menuDisappeared()), manager, SLOT(UnregisterMenu()));
}

MenuObject::~MenuObject()
{
    if(menu) delete menu;

    emit MenuUnregistered();
}

void MenuObject::SetItemActivity(const QString &itemId, bool isActive)
{
    qDebug() << "SetItemActivity";
}

void MenuObject::SetItemChecked(const QString &itemId, bool checked)
{
    qDebug() << "SetItemChecked";
}

void MenuObject::SetItemText(const QString &itemId, const QString &text)
{
    qDebug() << "SetItemText";
}

void MenuObject::ShowMenu(const QString &menuJsonContent)
{
    menu->setFlags(Qt::Tool | Qt::BypassWindowManagerHint);
    menu->setResizeMode(QQuickView::SizeViewToRootObject);
    menu->setColor(QColor(0, 0, 0, 0));
    menu->setSource(QUrl(QStringLiteral("qrc:///FullscreenBackground.qml")));
    menu->rootContext()->setContextProperty("_utils_", Utils::instance());
    menu->show();
    menu->requestActivate();

    QMetaObject::invokeMethod(menu->rootObject(), "showMenu", Q_ARG(QVariant, menuJsonContent));

    QQmlEngine *engine = menu->rootContext()->engine();
    QObject::connect(engine, SIGNAL(quit()), QGuiApplication::instance(), SLOT(quit()));

//    Utils::instance()->grabKeyboard(menu->winId());
//    Utils::instance()->grabPointer(menu->winId());
//    Utils::instance()->grabAll(menu->winId());
}
