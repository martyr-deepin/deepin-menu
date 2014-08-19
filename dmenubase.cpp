#include <QMargins>
#include <QtGlobal>
#include <QGraphicsDropShadowEffect>
#include <QJsonArray>
#include <QJsonObject>

#include "dmenubase.h"

DMenuBase::DMenuBase(QWidget *parent) :
    QWidget(parent, Qt::Popup),
    _radius(4),
    _shadowMargins(QMargins(0, 0, 0, 0))
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


void DMenuBase::setContent(const QJsonArray *items)
{
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
