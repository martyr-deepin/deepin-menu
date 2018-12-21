/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
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
#include <QScreen>

#include "ddockmenu.h"
#include "dmenucontent.h"
#include "utils.h"

DDockMenu::DDockMenu(DDockMenu *parent)
    : DArrowRectangle(DArrowRectangle::ArrowBottom, parent)
    , m_menuContent(new DMenuContent(this))
    , m_monitor(new DRegionMonitor(this))
{
    setAttribute(Qt::WA_InputMethodEnabled, false);

    setMouseTracking(true);

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

    connect(m_monitor, &DRegionMonitor::buttonPress, this, [=] (const QPoint &p) {
        if (geometry().contains(p)) {
            // The action performed is not from QAction and needs to be postponed because the menu requires a hover style.
            QTimer::singleShot(100, this, [=] {
                m_menuContent->processButtonClick(p);
            });
        } else {
            qDebug() << "window deactivate, destroy menu";
            destroyAll();
        }
    });
}

DDockMenu::~DDockMenu()
{
    m_monitor->unregisterRegion();
    setVisible(false);
    releaseFocus();
    releaseKeyboard();
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
        // NOTE(sbw): test if we have mouse handle
        if (rect().contains(mapFromGlobal(QCursor::pos())))
        {
            activateWindow();
        } else {
            qDebug() << "window deactivate, destroy menu";
            destroyAll();
        }
    }

    if (event->type() == QEvent::Move) {
        qDebug() << pos();
    }

    return DArrowRectangle::event(event);
}

void DDockMenu::showEvent(QShowEvent *e)
{
    Q_ASSERT(!m_monitor->registered());
    m_monitor->registerRegion();

    QTimer::singleShot(100, this, [=] {
        if (!isVisible())
            return;
        if (!isActiveWindow())
            activateWindow();

        grabKeyboard();
    });

    DArrowRectangle::showEvent(e);
}

void DDockMenu::hideEvent(QHideEvent *event)
{
    DArrowRectangle::hideEvent(event);

    m_monitor->unregisterRegion();
    releaseKeyboard();
}

void DDockMenu::mouseMoveEvent(QMouseEvent *event)
{
    DArrowRectangle::mouseMoveEvent(event);

    m_menuContent->processCursorMove(mapToGlobal(event->pos()));
}

void DDockMenu::keyPressEvent(QKeyEvent *event)
{
    DArrowRectangle::keyPressEvent(event);

    switch (event->key()) {
    case Qt::Key_Escape:
        destroyAll();
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        m_menuContent->doCurrentAction();
        break;
    case Qt::Key_Up:
        m_menuContent->selectPrevious();
        break;
    case Qt::Key_Down:
        m_menuContent->selectNext();
        break;
    default:
        break;
    }
}

/**
 * @brief DDockMenu::menuUnderPoint
 * @param point is a global position.
 * @return
 */
DDockMenu *DDockMenu::menuUnderPoint(const QPoint point)
{
    return geometry().contains(point) ? this : nullptr;
}

void DDockMenu::releaseFocus()
{
    qDebug() << Q_FUNC_INFO << this;

    releaseMouse();
    releaseKeyboard();
}

void DDockMenu::destroyAll()
{
    // NOTE(hualet): the events processed by this menu is actually delivered by
    // xmousearea which is xrecord backed, so if we destroy this window too
    // early, say immediately after mouse clicks, the actual events will go to
    // the window behide the menu(desktop for example).
    QTimer::singleShot(100, this, [this] {
        qApp->processEvents();
        deleteLater();
    });
}

void DDockMenu::onWMCompositeChanged()
{
    if (m_wmHelper->hasComposite())
        setBorderColor(QColor(255, 255, 255, 0));
    else
        setBorderColor(QColor("#2C3238"));
}
