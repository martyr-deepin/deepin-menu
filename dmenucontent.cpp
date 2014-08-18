#include <QRect>
#include <QPainter>
#include <QBrush>
#include <QAction>
#include <QtGlobal>
#include <QMouseEvent>
#include <QDebug>

#include "dmenubase.h"
#include "dmenucontent.h"

DMenuContent::DMenuContent(QWidget *parent) :
    QWidget(parent),
    _currentIndex(-1)
{
    this->setMouseTracking(true);

    this->addAction(new QAction("hello hello", this));
    this->addAction(new QAction("hello", this));
    this->addAction(new QAction("hello", this));
    this->addAction(new QAction("hello", this));
    this->addAction(new QAction("hello", this));
    this->addAction(new QAction("hello", this));
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

    QFont font;
    font.setPixelSize(MENU_ITEM_FONT_SIZE);
    QFontMetrics metrics(font);
    foreach (QAction *action, this->actions()) {
        result = qMax(result, metrics.width(action->text()));
    }
    return result;
}

int DMenuContent::contentHeight()
{
    return this->actions().count() * MENU_ITEM_HEIGHT;
}

// override methods
void DMenuContent::paintEvent(QPaintEvent * event)
{
    DMenuBase *parent = dynamic_cast<DMenuBase*>(this->parent());
    QFont font;
    font.setPixelSize(MENU_ITEM_FONT_SIZE);
    QPainter painter(this);
    painter.setFont(font);

    for(int i = 0; i < this->actions().count(); i++) {
        DMenuBase::ItemState itemState = i == _currentIndex ? DMenuBase::HoverState : DMenuBase::NormalState;
        parent->setItemState(itemState);

        QRect actionRect = this->getRectOfActionAtIndex(i);
        QRect actionRectWidthMargins = actionRect.marginsRemoved(QMargins(this->contentsMargins().left(), 0, this->contentsMargins().right(), 0));
        painter.fillRect(actionRect, QBrush(parent->itemBackgroundColor()));
        painter.setPen(QPen(parent->itemTextColor()));
        painter.drawText(actionRectWidthMargins, Qt::AlignVCenter, this->actions().at(i)->text());
    }
}

void DMenuContent::mouseMoveEvent(QMouseEvent *event)
{
    _currentIndex = event->y() / MENU_ITEM_HEIGHT;
    this->update();
}

// private methods
QRect DMenuContent::getRectOfActionAtIndex(int index)
{
    return QRect(0, index * MENU_ITEM_HEIGHT, this->width(), MENU_ITEM_HEIGHT);
}
