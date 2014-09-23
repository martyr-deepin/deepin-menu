#include <QPainter>
#include <QPainterPath>
#include <QBrush>
#include <QJsonObject>
#include <QJsonArray>
#include <QtGlobal>
#include <QDebug>

#include "utils.h"
#include "dmenubase.h"
#include "ddesktopmenu.h"
#include "dmenucontent.h"

DDesktopMenu::DDesktopMenu(DDesktopMenu *parent):
    DMenuBase(parent)
{
    this->setShadowMargins(QMargins(10, 10, 10, 10));
    this->setContentsMargins(this->shadowMargins());
    this->setMenuContentMargins(QMargins(5, 5, 5, 5));
    this->setItemLeftSpacing(10);
    this->setItemCenterSpacing(20);
    this->setItemRightSpacing(20);

    this->_normalStyle = { Qt::transparent,
                           Qt::black,
                           QColor("#636363"),
                           ":/images/check_light_normal.png",
                           ":/images/arrow-light.png" };
    this->_hoverStyle = { QColor("#535353"),
                          Qt::white,
                          QColor("#636363"),
                          ":/images/check_light_hover.png",
                          ":/images/arrow-light-hover.png" };
    this->_inactiveStyle = { Qt::transparent,
                             QColor("#b4b4b4"),
                             QColor("#636363"),
                             ":/images/check_light_inactive.png",
                             ":/images/arrow-light.png" };

    QSharedPointer<DMenuContent> ptr(new DMenuContent(this));
    this->setMenuContent(ptr);
}

// override methods
void DDesktopMenu::paintEvent(QPaintEvent *)
{
    QRect rect = this->rect().marginsRemoved(shadowMargins());

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath border;
    border.addRoundedRect(rect, radius(), radius());

    painter.fillPath(border, QBrush(Qt::white));
}

void DDesktopMenu::setPosition(int x, int y)
{
    DMenuBase *rootMenu = this->getRootMenu();
    Q_ASSERT(rootMenu);

    QPoint point(x - this->shadowMargins().left(),
                 y - this->shadowMargins().top());
    QRect currentMonitorRect = (this == rootMenu) ?  Utils::currentMonitorRect(x, y)
                                                   : Utils::currentMonitorRect(rootMenu->x() + rootMenu->shadowMargins().left(),
                                                                               rootMenu->y() + rootMenu->shadowMargins().top());

    if (point.x()
            + this->width()
            > currentMonitorRect.x()
            + currentMonitorRect.width()) {
        if (this->parent()) {
            DMenuBase *parent = qobject_cast<DMenuBase *>(this->parent());
            Q_ASSERT(parent);

            point.setX(parent->x()
                       + parent->shadowMargins().left()
                       - this->width()
                       + this->shadowMargins().right());
        } else {
            point.setX(point.x()
                       - this->width()
                       + this->shadowMargins().left()
                       + this->shadowMargins().right());
        }
    }

    if (point.y()
            + this->height()
            > currentMonitorRect.y()
            + currentMonitorRect.height()) {
        point.setY(currentMonitorRect.y()
                   + currentMonitorRect.height()
                   - this->height());

    }

    this->move(point);
}

void DDesktopMenu::showSubMenu(int x, int y, QJsonObject subMenuJsonObject)
{
    QJsonArray items = subMenuJsonObject["items"].toArray();
    if (items.count() != 0) {
        if (!_subMenu) {
            _subMenu = new DDesktopMenu(this);
        }
        _subMenu->setContent(items);
        _subMenu->setPosition(x, y);
        _subMenu->show();
    } else if (_subMenu && _subMenu->isVisible()) {
        _subMenu->deleteLater();
        _subMenu = NULL;
    }
}
