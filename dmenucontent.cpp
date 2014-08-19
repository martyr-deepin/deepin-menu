#include <QRect>
#include <QPainter>
#include <QBrush>
#include <QAction>
#include <QtGlobal>
#include <QMouseEvent>
#include <QIcon>
#include <QColor>
#include <QDebug>

#include "dmenubase.h"
#include "dmenucontent.h"

DMenuContent::DMenuContent(DMenuBase *parent) :
    QWidget(parent),
    _currentIndex(-1)
{
    this->setMouseTracking(true);
}

int DMenuContent::currentIndex()
{
    return _currentIndex;
}

void DMenuContent::setCurrentIndex(int index)
{
    _currentIndex = index;
}

int DMenuContent::contentWidth()
{
    int result = 0;
    _iconWidth = 0;
    _shortcutWidth = 0;
    _subMenuIndicatorWidth = 0;
    DMenuBase *parent = qobject_cast<DMenuBase*>(this->parent());

    QFont font;
    font.setPixelSize(MENU_ITEM_FONT_SIZE);
    QFontMetrics metrics(font);
    foreach (QAction *action, this->actions()) {
        if(!action->property("itemIcon").toString().isEmpty() || \
           !action->property("itemIconHover").toString().isEmpty() || \
           !action->property("itemIconInactive").toString().isEmpty() || \
           action->isCheckable()) {
            _iconWidth = MENU_ICON_SIZE + parent->itemLeftSpacing();
        }
        if(!action->shortcut().isEmpty()) _shortcutWidth = qMax(_shortcutWidth, metrics.width(action->shortcut().toString()) + parent->itemCenterSpacing());
        if(action->property("hasSubMenu").toBool()) _subMenuIndicatorWidth = SUB_MENU_INDICATOR_SIZE + parent->itemRightSpacing();

        result = qMax(result, metrics.width(action->text()));
    }

    return result + _iconWidth + _shortcutWidth + _subMenuIndicatorWidth;
}

int DMenuContent::contentHeight()
{
    int result = 0;

    foreach (QAction *action, this->actions()) {
        if (action->text().isEmpty()) {
            result += SEPARATOR_HEIGHT;
        } else {
            result += MENU_ITEM_HEIGHT;
        }
    }

    return result;
}

// override methods
void DMenuContent::paintEvent(QPaintEvent * event)
{
    DMenuBase *parent = qobject_cast<DMenuBase*>(this->parent());
    QFont font;
    font.setPixelSize(MENU_ITEM_FONT_SIZE);
    QPainter painter(this);
    painter.setFont(font);

    for(int i = 0; i < this->actions().count(); i++) {
    	QAction *action = this->actions().at(i);
        DMenuBase::ItemState itemState = i == _currentIndex ? DMenuBase::HoverState : DMenuBase::NormalState;
        parent->setItemState(itemState);

        QRect actionRect = this->getRectOfActionAtIndex(i);
        QRect actionRectWidthMargins = actionRect.marginsRemoved(QMargins(this->contentsMargins().left(), 0, this->contentsMargins().right(), 0));

        // indicates that this item is a separator
        if (action->text().isEmpty()) {
            int topLineX1 = actionRect.x();
            int topLineY1 = actionRect.y() + (actionRect.height() - 2) / 2;
            int topLineX2 = actionRect.x() + actionRect.width();
            int topLineY2 = actionRect.y() + (actionRect.height() - 2) / 2;
            int bottomLineX1 = topLineX1;
            int bottomLineY1 = topLineY1 + 1;
            int bottomLineX2 = topLineX2;
            int bottomLineY2 = topLineY1 + 1;
            painter.setPen(QPen(QColor::fromRgbF(0, 0, 0, 0.2)));
            painter.drawLine(topLineX1, topLineY1, topLineX2, topLineY2);
            painter.setPen(QPen(QColor::fromRgbF(1, 1, 1, 0.1)));
            painter.drawLine(bottomLineX1, bottomLineY1, bottomLineX2, bottomLineY2);
        } else {
            painter.fillRect(actionRect, QBrush(parent->itemBackgroundColor()));
            painter.setPen(QPen(parent->itemTextColor()));

            // draw icon
            if(_iconWidth) {
                QRect iconRect = QRect(actionRectWidthMargins.x(),
                                       actionRectWidthMargins.y() + (MENU_ITEM_HEIGHT - MENU_ICON_SIZE) / 2,
                                       MENU_ICON_SIZE, MENU_ICON_SIZE);

                if (!action->property("itemIcon").isNull() || \
                    !action->property("itemIconHover").isNull() || \
                    !action->property("itemIconInactive").isNull())
                {
                    painter.drawPixmap(iconRect, action->icon().pixmap(MENU_ICON_SIZE, MENU_ICON_SIZE));
                } else if (action->isCheckable()) {
                    painter.drawImage(iconRect, QImage(parent->checkmarkIcon()));
                }
            }

            // draw text
            painter.drawText(QRect(actionRectWidthMargins.x() + _iconWidth,
                                   actionRectWidthMargins.y(),
                                   actionRectWidthMargins.width(),
                                   actionRectWidthMargins.height()),
                             Qt::AlignVCenter,
                             action->text());

            // draw shortcut text
            if(_shortcutWidth)
                painter.drawText(QRect(actionRectWidthMargins.x(),
                                       actionRectWidthMargins.y(),
                                       actionRectWidthMargins.width() - _subMenuIndicatorWidth,
                                       actionRectWidthMargins.height()),
                                 Qt::AlignVCenter | Qt::AlignRight,
                                 action->shortcut().toString());

            // draw sub menu indicator icon
            if(action->property("hasSubMenu").toBool())
                painter.drawImage(QRect(actionRectWidthMargins.x() + actionRectWidthMargins.width() - SUB_MENU_INDICATOR_SIZE,
                                        actionRectWidthMargins.y() + (MENU_ITEM_HEIGHT - MENU_ICON_SIZE) / 2,
                                        SUB_MENU_INDICATOR_SIZE, SUB_MENU_INDICATOR_SIZE),
                                  QImage(parent->subMenuIndicatorIcon()));
        }
    }
}

void DMenuContent::mouseMoveEvent(QMouseEvent *event)
{
    int previousHeight = this->rect().y();

    for (int i = 0; i < this->actions().count(); i++) {
        QAction *action = this->actions().at(i);
        int itemHeight = this->actions().at(i)->text().isEmpty() ? SEPARATOR_HEIGHT : MENU_ITEM_HEIGHT;

        if (previousHeight <= event->y() && event->y() <+ previousHeight + itemHeight) {
            _currentIndex = i; break;
        } else {
            previousHeight += itemHeight;
        }
    }

    this->update();
}

// private methods
QRect DMenuContent::getRectOfActionAtIndex(int index)
{
    int previousHeight = 0;
    int itemHeight = this->actions().at(index)->text().isEmpty() ? SEPARATOR_HEIGHT : MENU_ITEM_HEIGHT;

    for (int i = 0; i < index; i++) {
        QAction *action = this->actions().at(i);
        if (action->text().isEmpty()) {
            previousHeight += SEPARATOR_HEIGHT;
        } else {
            previousHeight += MENU_ITEM_HEIGHT;
        }
    }

    return QRect(0, previousHeight, this->width(), itemHeight);
}
