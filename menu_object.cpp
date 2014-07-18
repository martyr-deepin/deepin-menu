#include <QDebug>

#include "menu_object.h"

MenuObject::MenuObject(QObject *parent) :
    QObject(parent)
{
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
    qDebug() << "ShowMenu";
}
