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

#include <QPainter>
#include <QPainterPath>
#include <QRect>
#include <QPen>
#include <QBrush>
#include <QHBoxLayout>
#include <QJsonObject>
#include <QDebug>
#include <QJsonArray>
#include <QApplication>

#include "ddockmenu.h"
#include "dmenucontent.h"
#include "utils.h"

DDockMenu::DDockMenu(DDockMenu *parent):
    DArrowRectangle(DArrowRectangle::ArrowBottom, parent),
    m_menuContent(new DMenuContent(this)),
    m_mouseAreaInter(new com::deepin::api::XMouseArea("com.deepin.api.XMouseArea",
                                                      "/com/deepin/api/XMouseArea",
                                                      QDBusConnection::sessionBus(), this))
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);

    m_wmHelper = DWindowManagerHelper::instance();

    connect(m_wmHelper, &DWindowManagerHelper::hasCompositeChanged, this, &DDockMenu::onWMCompositeChanged);

    onWMCompositeChanged();

    setAccessibleName("DockMenu");
    setBackgroundColor(DBlurEffectWidget::DarkColor);
    setMargin(0);
    setArrowWidth(18);
    setArrowHeight(10);

    normalStyle = ItemStyle{Qt::transparent,
            Qt::white,
            QColor("#646464"),
            ":/images/check_dark_normal.png",
            ":/images/arrow-dark.png"};
    hoverStyle = ItemStyle{QColor("#2ca7f8"),
            Qt::white,
            QColor("#646464"),
            ":/images/check_dark_hover.png",
            ":/images/arrow-dark.png"};
    inactiveStyle = ItemStyle{Qt::transparent,
            QColor("#646464"),
            QColor("#646464"),
            ":/images/check_dark_inactive.png",
            ":/images/arrow-dark.png"};

    connect(m_mouseAreaInter, &__XMouseArea::ButtonPress, this, &DDockMenu::onButtonPress);
    connect(m_mouseAreaInter, &__XMouseArea::CursorMove, this, &DDockMenu::onCursorMove);
    connect(m_mouseAreaInter, &__XMouseArea::KeyPress, this, &DDockMenu::onKeyPress);
}

DDockMenu::~DDockMenu()
{
    releaseFocus();
}

void DDockMenu::setItems(QJsonArray items)
{
    m_menuContent->setCurrentIndex(-1);
    m_menuContent->clearActions();

    foreach (QJsonValue item, items) {
        QJsonObject itemObj = item.toObject();

        QAction *action = new QAction(m_menuContent);
        QString itemText = itemObj["itemText"].toString().replace("_", QString()).replace(QRegExp("\\([^)]+\\)"), QString());

        action->setText(itemText);
        action->setEnabled(itemObj["isActive"].toBool());
        action->setCheckable(itemObj["isCheckable"].toBool() || Utils::menuItemCheckableFromId(itemObj["itemId"].toString()));
        action->setChecked(itemObj["checked"].toBool());
        action->setProperty("itemId", itemObj["itemId"].toString());
        action->setProperty("itemIcon", itemObj["itemIcon"].toString());
        action->setProperty("itemIconHover", itemObj["itemIconHover"].toString());
        action->setProperty("itemIconInactive", itemObj["itemIconInactive"].toString());
        action->setProperty("itemSubMenu", itemObj["itemSubMenu"].toObject());

        QRegExp regexp("_(.)");
        regexp.indexIn(itemObj["itemText"].toString());
        QString navKey = regexp.cap(1);
        QString navKeyWrapper = QString("%1").arg(navKey);
        itemText = itemObj["itemText"].toString().replace(regexp, navKeyWrapper);
        action->setText(itemText);
        action->setProperty("itemNavKey", navKey);

        m_menuContent->addAction(action);
    }

    // adjust its size according to its content.
    m_menuContent->setFixedSize(m_menuContent->contentWidth(),
                                m_menuContent->contentHeight());


    setContent(m_menuContent);

    resizeWithContent();
}

DDockMenu *DDockMenu::getRootMenu()
{
    return this;
}

void DDockMenu::showSubMenu(int, int, const QJsonObject &)
{

}

bool DDockMenu::event(QEvent *event)
{
    if (event->type() == QEvent::WindowDeactivate) {
        destroyAll();
    }

    return DArrowRectangle::event(event);
}

/**
 * @brief DDockMenu::menuUnderPoint
 * @param point is a global position.
 * @return
 */
DDockMenu *DDockMenu::menuUnderPoint(const QPoint point)
{
    const qreal ratio = qApp->devicePixelRatio();

    QRect rect( geometry().x() * ratio,
                geometry().y() * ratio,
                geometry().width() * ratio,
                geometry().height() * ratio);

    if (rect.contains(point)) {
        return this;
    }

    return nullptr;
}

void DDockMenu::grabFocus()
{
    // Try to make us the focus grabber window, so that tooltips
    // on Dock will disappear.
    QTimer::singleShot(500, this, [this] {
        activateWindow();
        grabMouse();
        grabKeyboard();
    });

    QDBusPendingCall call = m_mouseAreaInter->RegisterFullScreen();
    QDBusPendingCallWatcher *watcher = new QDBusPendingCallWatcher(call, this);
    connect(watcher, &QDBusPendingCallWatcher::finished, this, [this, watcher] {
        if (watcher->isError()) {
            qWarning() << "error registering mouse area: " << watcher->error().message();
        } else {
            QDBusReply<QString> reply = watcher->reply();
            m_mouseAreaKey = reply.value();
        }
    });
}

void DDockMenu::releaseFocus()
{
    releaseMouse();
    releaseKeyboard();
    m_mouseAreaInter->UnregisterArea(m_mouseAreaKey);
}

void DDockMenu::destroyAll()
{
    // NOTE(hualet): the events processed by this menu is actually delivered by
    // xmousearea which is xrecord backed, so if we destroy this window too
    // early, say immediately after mouse clicks, the actual events will go to
    // the window behide the menu(desktop for example).
    QTimer::singleShot(100, this, [this] {
        deleteLater();
    });
}

void DDockMenu::onButtonPress(int, int in1, int in2, const QString &in3)
{
    if (in3 == m_mouseAreaKey) {
        qDebug() << "receive button press event from xmousearea: " << in1 << in2;
        m_menuContent->processButtonClick(in1, in2);
    }
}

void DDockMenu::onCursorMove(int in0, int in1, const QString &in2)
{
    if (in2 == m_mouseAreaKey) {
        m_menuContent->processCursorMove(in0, in1);
    }
}

void DDockMenu::onKeyPress(const QString &in0, int, int, const QString &in3)
{
    if (in3 == m_mouseAreaKey) {
        qDebug() << "receive key press event from xmousearea: " << in0;
        m_menuContent->processKeyPress(in0);
    }
}

void DDockMenu::onWMCompositeChanged()
{
    if (m_wmHelper->hasComposite())
        setBorderColor(QColor(255, 255, 255, 0.1 * 255));
    else
        setBorderColor(QColor("#2C3238"));
}
