#include <QRect>
#include <QPainter>
#include <QBrush>

#include "dmenucontent.h"

DMenuContent::DMenuContent(QWidget *parent) :
    QWidget(parent)
{
    this->setGeometry(0, 0, contentWidth(), contentHeight());
}

int DMenuContent::contentWidth()
{
    return 100;
}

int DMenuContent::contentHeight()
{
    return 100;
}

// override methods
void DMenuContent::paintEvent(QPaintEvent * event)
{
    QRect rect = this->rect();

    QPainter painter(this);
    painter.fillRect(rect, QBrush(Qt::green));
}
