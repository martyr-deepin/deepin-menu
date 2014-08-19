#include <QMargins>
#include <QtGlobal>
#include <QGraphicsDropShadowEffect>
#include <QJsonArray>
#include <QJsonObject>
#include <QHBoxLayout>
#include <QStringList>
#include <QSharedPointer>
#include <QDebug>

#include "dmenubase.h"
#include <dmenucontent.h>

DMenuBase::DMenuBase(QWidget *parent) :
    QWidget(parent, Qt::Popup),
    _radius(4),
    _shadowMargins(QMargins(0, 0, 0, 0)),
    _checkmarkIcon(""),
    _subMenuIndicatorIcon("")
{
    this->setAttribute(Qt::WA_TranslucentBackground);

    _dropShadow = new QGraphicsDropShadowEffect(this);
    _dropShadow->setBlurRadius(0);
    _dropShadow->setColor(Qt::black);
    _dropShadow->setOffset(0.0);
    this->setGraphicsEffect(_dropShadow);
}

// getters and setters
int DMenuBase::radius()
{
    return _radius;
}

void DMenuBase::setRadius(int radius)
{
    if (_radius != radius) {
        _radius = radius;

        emit radiusChanged(radius);
    }
}

QMargins DMenuBase::shadowMargins()
{
    return _shadowMargins;
}

void DMenuBase::setShadowMargins(QMargins shadowMargins)
{
    if (_shadowMargins != shadowMargins) {
        _shadowMargins = shadowMargins;
        _dropShadow->setBlurRadius(qMax(qMax(_shadowMargins.left(), _shadowMargins.top()),
                                        qMax(_shadowMargins.right(), _shadowMargins.bottom())));

        emit shadowMarginsChanged(shadowMargins);
    }
}

QMargins DMenuBase::menuContentMargins()
{
    return _menuContentMargins;
}

void DMenuBase::setMenuContentMargins(QMargins margins)
{
    if(_menuContentMargins != margins) {
        _menuContentMargins = margins;

        emit menuContentMarginsChanged(margins);
    }
}

QColor DMenuBase::itemBackgroundColor()
{
    return _itemBackgroundColor;
}

void DMenuBase::setItemBackgroundColor(QColor color)
{
    if (_itemBackgroundColor != color) {
        _itemBackgroundColor = color;

        emit itemBackgroundColorChanged(color);
    }
}

QColor DMenuBase::itemTextColor()
{
    return _itemTextColor;
}

void DMenuBase::setItemTextColor(QColor color)
{
    if (_itemTextColor != color) {
        _itemTextColor = color;

        emit itemTextColorChanged(color);
    }
}

QColor DMenuBase::itemShortcutColor()
{
    return _itemShortcutColor;
}

void DMenuBase::setItemShortcutColor(QColor color)
{
    if (_itemShortcutColor != color) {
        _itemShortcutColor = color;

        emit itemShortcutColorChanged(color);
    }
}

int DMenuBase::itemLeftSpacing()
{
    return _itemLeftSpacing;
}

void DMenuBase::setItemLeftSpacing(int spacing)
{
    if (_itemLeftSpacing != spacing) {
        _itemLeftSpacing = spacing;

        emit itemLeftSpacingChanged(spacing);
    }
}

int DMenuBase::itemCenterSpacing()
{
    return _itemCenterSpacing;
}

void DMenuBase::setItemCenterSpacing(int spacing)
{
    if (_itemCenterSpacing != spacing) {
        _itemCenterSpacing = spacing;

        emit itemCenterSpacingChanged(spacing);
    }
}

int DMenuBase::itemRightSpacing()
{
    return _itemRightSpacing;
}

void DMenuBase::setItemRightSpacing(int spacing)
{
    if (_itemRightSpacing != spacing) {
        _itemRightSpacing = spacing;

        emit itemRightSpacingChanged(spacing);
    }
}

QString DMenuBase::checkmarkIcon()
{
    return _checkmarkIcon;
}

void DMenuBase::setCheckmarkIcon(QString icon)
{
    if(icon != _checkmarkIcon) {
        _checkmarkIcon = icon;
    }
}

QString DMenuBase::subMenuIndicatorIcon()
{
    return _subMenuIndicatorIcon;
}

void DMenuBase::setSubMenuIndicatorIcon(QString icon)
{
    if(icon != _subMenuIndicatorIcon) {
        _subMenuIndicatorIcon = icon;
    }
}

QSharedPointer<DMenuContent> DMenuBase::menuContent()
{
    return _menuContent;
}

void DMenuBase::setMenuContent(QSharedPointer<DMenuContent> content)
{
    _menuContent = content;
    _menuContent->setContentsMargins(_menuContentMargins);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(QMargins(0, _menuContentMargins.top(), 0, _menuContentMargins.bottom()));
    layout->addWidget(_menuContent.data());

    this->setLayout(layout);
}

void DMenuBase::setContent(QJsonArray items)
{
    Q_ASSERT(this->menuContent());
    this->menuContent()->actions().clear();

    foreach (QJsonValue item, items) {
        QJsonObject itemObj = item.toObject();

        QAction *action = new QAction(this->menuContent().data());
        action->setText(itemObj["itemText"].toString());
        action->setEnabled(itemObj["isActive"].toBool());
        action->setCheckable(menuItemCheckableFromId(itemObj["itemId"].toString()));
        action->setChecked(itemObj["checked"].toBool());
        action->setProperty("itemId", itemObj["itemId"].toString());
        action->setProperty("itemIcon", itemObj["itemIcon"].toString());
        action->setProperty("itemIconHover", itemObj["itemIconHover"].toString());
        action->setProperty("itemIconInactive", itemObj["itemIconInactive"].toString());
        action->setProperty("hasSubMenu", itemObj["itemSubMenu"].toObject()["items"].toArray().count() != 0);

        _menuContent->addAction(action);
    }

    // adjust its size according to its content.
    this->resize(_menuContent->contentWidth()
                 + _shadowMargins.left()
                 + _shadowMargins.right()
                 + _menuContentMargins.left()
                 + _menuContentMargins.right(),
                 _menuContent->contentHeight()
                 + _shadowMargins.top()
                 + _shadowMargins.bottom()
                 + _menuContentMargins.top()
                 + _menuContentMargins.bottom());
}

void DMenuBase::destroyAll()
{
    if(this->parent()) {
        DMenuBase *parent = qobject_cast<DMenuBase*>(this->parent());
        Q_ASSERT(parent);
        parent->destroyAll();
    } else {
        this->deleteLater();
    }
}

bool DMenuBase::menuItemCheckableFromId(QString id)
{
    return id.split(':').count() == 3;

}
