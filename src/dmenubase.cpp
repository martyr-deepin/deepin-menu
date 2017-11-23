/*
 * Copyright (C) 2015 ~ 2017 Deepin Technology Co., Ltd.
 *
 * Author:     Hualet Wang <mr.asianwang@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QMargins>
#include <QtGlobal>
#include <QGraphicsDropShadowEffect>
#include <QJsonArray>
#include <QJsonObject>
#include <QHBoxLayout>
#include <QSharedPointer>
#include <QRegExp>
#include <QPoint>
#include <QWindow>
#include <QThread>
#include <QTimer>
#include <QTime>
#include <QDebug>
#include <QX11Info>

#include <X11/Xlib.h>

#include "dmenubase.h"
#include "dmenucontent.h"
#include "utils.h"

#define GRAB_FOCUS_TRY_TIMES 100

DMenuBase::DMenuBase(QWidget *parent) :
    QWidget(parent, Qt::Tool | Qt::BypassWindowManagerHint),
    _subMenu(NULL),
    _radius(4),
    _shadowMargins(QMargins(0, 0, 0, 0))
{
    this->setAttribute(Qt::WA_TranslucentBackground);

    queryXIExtension();

    _dropShadow = new QGraphicsDropShadowEffect(this);
    _dropShadow->setColor(QColor::fromRgbF(0, 0, 0, 0.2));
    _dropShadow->setXOffset(0);
    _dropShadow->setYOffset(6);
    this->setGraphicsEffect(_dropShadow);

    _grabFocusTimer = new QTimer(this);
    _grabFocusTimer->setSingleShot(true);
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

DMenuBase* DMenuBase::subMenu()
{
    return _subMenu;
}

void DMenuBase::setContent(QJsonArray items)
{
    Q_ASSERT(this->menuContent());
    this->menuContent()->setCurrentIndex(-1);
    this->menuContent()->clearActions();

    foreach (QJsonValue item, items) {
        QJsonObject itemObj = item.toObject();

        QAction *action = new QAction(this->menuContent().data());
        QRegExp regexp("_(.)");
        regexp.indexIn(itemObj["itemText"].toString());
        QString navKey = regexp.cap(1);
        QString navKeyWrapper = QString("<u>%1</u>").arg(navKey);
        QString itemText = itemObj["itemText"].toString().replace(regexp, navKeyWrapper);

        action->setText(itemText);
        action->setEnabled(itemObj["isActive"].toBool());
        action->setCheckable(itemObj["isCheckable"].toBool() || Utils::menuItemCheckableFromId(itemObj["itemId"].toString()));
        action->setChecked(itemObj["checked"].toBool());
        action->setProperty("itemId", itemObj["itemId"].toString());
        action->setProperty("itemIcon", itemObj["itemIcon"].toString());
        action->setProperty("itemIconHover", itemObj["itemIconHover"].toString());
        action->setProperty("itemIconInactive", itemObj["itemIconInactive"].toString());
        action->setProperty("itemSubMenu", itemObj["itemSubMenu"].toObject());
        action->setProperty("itemNavKey", navKey);

        _menuContent->addAction(action);
    }

    // adjust its size according to its content.
    this->resize(_menuContent->contentWidth()
                 + _shadowMargins.left()
                 + _shadowMargins.right()
                 + _menuContentMargins.left()
                 + _menuContentMargins.right()
                 + this->contentsMargins().left() - _shadowMargins.left()
                 + this->contentsMargins().right() - _shadowMargins.right(),
                 _menuContent->contentHeight()
                 + _shadowMargins.top()
                 + _shadowMargins.bottom()
                 + _menuContentMargins.top()
                 + _menuContentMargins.bottom()
                 + this->contentsMargins().top() - _shadowMargins.top()
                 + this->contentsMargins().bottom() - _shadowMargins.bottom());
}

void DMenuBase::destroyAll()
{
    releaseFocus();

    if(this->parent()) {
        DMenuBase *parent = qobject_cast<DMenuBase*>(this->parent());
        Q_ASSERT(parent);
        parent->destroyAll();
    } else {
        this->deleteLater();
    }
}

void DMenuBase::grabFocus()
{
    if (mouseGrabber() == menuContent().data()
            && keyboardGrabber() == menuContent().data())
    {
        // the menu already grabs the focus, no need
        // to do it again.
        return;
    }

    grabFocusSlot();
    connect(_grabFocusTimer, SIGNAL(timeout()), this, SLOT(grabFocusSlot()));
}

void DMenuBase::releaseFocus()
{
    if (mouseGrabber())
        mouseGrabber()->releaseMouse();
    if (keyboardGrabber())
        keyboardGrabber()->releaseKeyboard();
}

bool DMenuBase::grabFocusInternal(int tryTimes)
{
    QWindow *window = this->menuContent()->windowHandle();
    if (!window)
        if (const QWidget *nativeParent = this->menuContent()->nativeParentWidget())
            window = nativeParent->windowHandle();
    // grab pointer
    int i = 0;
    while(!window->setMouseGrabEnabled(true) && i < tryTimes) {
        QThread::msleep(10);
        i++;
    }
    if (i >= tryTimes) {
        qWarning() << QString("GrabMouse Failed after tring %1 times").arg(i);
    } else {
        qDebug() << QString("GrabMouse tries %1").arg(i);
    }
    this->menuContent()->grabMouse();

    // grab keyboard
    int j = 0;
    while(!window->setKeyboardGrabEnabled(true) && j < tryTimes) {
        QThread::msleep(10);
        j++;
    }
    if (j >= tryTimes) {
        qWarning() << QString("GrabKeyboard Failed after tring %1 times").arg(j);
    } else {
        qDebug() << QString("GrabKeyboard tries %1").arg(j);
    }
    this->menuContent()->grabKeyboard();

    return (i < tryTimes) && (j < tryTimes);
}

void DMenuBase::grabFocusSlot()
{
    if (!grabFocusInternal(1)) {
        _grabFocusTimer->start();
    }
}

DMenuBase *DMenuBase::menuUnderPoint(QPoint point)
{
    if(this->parent()) {
        DMenuBase *parent = qobject_cast<DMenuBase*>(this->parent());
        Q_ASSERT(parent);

        return parent->menuUnderPoint(point);
    } else {
        DMenuBase *result = NULL;
        DMenuBase *subMenu =this;
        while (subMenu) {
            // remove the shadow margin, so clicks on the shadow won't be eaten by us.
            if (Utils::pointInRect(point, subMenu->geometry().marginsRemoved(subMenu->shadowMargins()))) {
                // shouldn't return here, otherwise the old menus are able to steal focus from
                // the younger ones even if they are at the bottom of the stack.
                result = subMenu;
            }
            subMenu = subMenu->subMenu();
        }
        return result;
    }
}

void DMenuBase::setItemActivity(const QString &itemId, bool isActive)
{
    QString prop("%1Activity");
    this->setProperty(prop.arg(itemId).toLatin1(), isActive);
    this->updateAll();
}

void DMenuBase::setItemChecked(const QString &itemId, bool checked)
{
    QString prop("%1Checked");
    this->setProperty(prop.arg(itemId).toLatin1(), checked);
    this->updateAll();
}

void DMenuBase::setItemText(const QString &itemId, const QString &text)
{
    QString prop("%1Text");
    this->setProperty(prop.arg(itemId).toLatin1(), text);
    this->updateAll();
}

const DMenuBase::ItemStyle DMenuBase::normalStyle()
{
    return _normalStyle;
}

const DMenuBase::ItemStyle DMenuBase::hoverStyle()
{
    return _hoverStyle;
}

const DMenuBase::ItemStyle DMenuBase::inactiveStyle()
{
    return _inactiveStyle;
}

DMenuBase *DMenuBase::getRootMenu()
{
    QObject *root = this;
    while (root) {
        if (root->parent()) {
            root = root->parent();
        } else {
            break;
        }
    }

    DMenuBase *result = qobject_cast<DMenuBase *>(root);
    Q_ASSERT(result);

    return result;
}

// override methods
bool DMenuBase::nativeEvent(const QByteArray &eventType, void *message, long *)
{
    if (eventType=="xcb_generic_event_t") {
        xcb_generic_event_t *event = static_cast<xcb_generic_event_t*>(message);
        const uint8_t responseType = event->response_type & ~0x80;

        xXIDeviceEvent *ev = reinterpret_cast<xXIDeviceEvent*>(event);
        //            qDebug() << ev->detail << Button1Mask << Button3Mask << Button2Mask;
        if (ev->detail !=0 && ev->detail != 1 && ev->detail != 2 && ev->detail != 3) {
            // drop all mouse events with button other than left button
            // or right button;
            return false;
        }

        if (isXIType(event, xiOpCode, XI_ButtonPress) || isXIType(event, xiOpCode, XI_TouchBegin)) {
            //                qDebug() << "nativeEvent XI_ButtonPress" << fixed1616ToReal(ev->root_x) <<
            //                    fixed1616ToReal(ev->root_y);
            if (!this->menuUnderPoint(QPoint(fixed1616ToReal(ev->root_x),
                                             fixed1616ToReal(ev->root_y)))) {
                this->destroyAll();
            }
        } else if (isXIType(event, xiOpCode, XI_ButtonRelease)) {
            //                qDebug() << "nativeEvent XI_ButtonRelease" << fixed1616ToReal(ev->root_x) <<
            //                    fixed1616ToReal(ev->root_y);
            if (this->menuUnderPoint(QPoint(fixed1616ToReal(ev->root_x),
                                            fixed1616ToReal(ev->root_y))) && _menuContent){
                _menuContent->doCurrentAction();
            }
        } else if (isXIType(event, xiOpCode, XI_Motion)) {
            //                qDebug() << "nativeEvent XI_Motion" << fixed1616ToReal(ev->root_x) <<
            //                    fixed1616ToReal(ev->root_y);
            DMenuBase *menuUnderPoint = this->menuUnderPoint(
                        QPoint(fixed1616ToReal(ev->root_x), fixed1616ToReal(ev->root_y)));
            if (menuUnderPoint) {
                menuUnderPoint->grabFocus();
            }
        }
    }

    return false;
}

void DMenuBase::queryXIExtension()
{
    XQueryExtension((Display *)QX11Info::display(), "XInputExtension", &xiOpCode, &xiEventBase, &xiErrorBase);
    qDebug() << "xiOpCode: " << xiOpCode;
}

bool DMenuBase::isXIEvent(xcb_generic_event_t *event, int opCode)
{
    qt_xcb_ge_event_t *e = (qt_xcb_ge_event_t *)event;
    return e->extension == opCode;
}

bool DMenuBase::isXIType(xcb_generic_event_t *event, int opCode, uint16_t type)
{
    if (!isXIEvent(event, opCode))
        return false;

    xXIGenericDeviceEvent *xiEvent = reinterpret_cast<xXIGenericDeviceEvent *>(event);
    return xiEvent->evtype == type;
}

qreal DMenuBase::fixed1616ToReal(FP1616 val)
{
    return (qreal(val >> 16)) + (val & 0xFFFF) / (qreal)0xFFFF;
}

// private methods
void DMenuBase::updateAll()
{
    DMenuBase *subMenu = this;
    while (subMenu) {
        subMenu->update();
        subMenu = subMenu->subMenu();
    }
}
