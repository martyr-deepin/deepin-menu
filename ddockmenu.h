#ifndef DDOCKMENU_H
#define DDOCKMENU_H

#include "dmenubase.h"

#define CORNER_WIDTH 18
#define CORNER_HEIGHT 10

class DDockMenu : public DMenuBase
{
public:
    DDockMenu();

    void setItemState(ItemState);

protected:
    void paintEvent(QPaintEvent *);
};

#endif // DDOCKMENU_H
