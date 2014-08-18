#include <QRect>
#include <QPainter>
#include <QBrush>
#include <QAction>
#include <QtGlobal>

#include "ddesktopmenu.h"
#include "dmenucontent.h"

DMenuContent::DMenuContent(QWidget *parent) :
    QWidget(parent)
{
    this->setGeometry(0, 0, contentWidth(), contentHeight());

    this->addAction(new QAction("hello", this));
    this->addAction(new QAction("hello", this));
    this->addAction(new QAction("hello", this));
    this->addAction(new QAction("hello", this));
    this->addAction(new QAction("hello", this));
    this->addAction(new QAction("hello", this));
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
    DDesktopMenu *parent = dynamic_cast<DDesktopMenu*>(this->parent());
    QRect rect = this->rect();
    QFont font;
    font.setPixelSize(MENU_ITEM_FONT_SIZE);
    QPainter painter(this);
    painter.setFont(font);

    DMenuBase::ItemState itemState = DMenuBase::NormalState;
    parent->setItemState(itemState);

    for(int i = 0; i < this->actions().count(); i++) {
        painter.drawText(this->getRectOfActionAtIndex(i), Qt::AlignVCenter, this->actions().at(i)->text());
    }
}

// private methods
QRect DMenuContent::getRectOfActionAtIndex(int index)
{
    return QRect(0, index * MENU_ITEM_HEIGHT, this->width(), MENU_ITEM_HEIGHT);
}
