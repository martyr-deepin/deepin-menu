#include "menu.h"
#include <QQmlContext>

Menu::Menu()
{
    this->setFlags(Qt::FramelessWindowHint | Qt::Tool);
    this->setResizeMode(QQuickView::SizeViewToRootObject);
    this->setSource(QUrl(QStringLiteral("qrc:///DesktopMenu.qml")));

    this->rootContext()->setContextProperty("_menu_", this);
}

Menu::~Menu()
{
    if(this->childMenu) delete this->childMenu;
}
