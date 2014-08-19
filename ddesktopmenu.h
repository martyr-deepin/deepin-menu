#ifndef DDESKTOPMENU_H
#define DDESKTOPMENU_H

#include <dmenubase.h>

class DDesktopMenu : public DMenuBase
{
public:
    DDesktopMenu();

    virtual void setItemState(ItemState);
    virtual void setPosition(int, int);

protected:
    virtual void paintEvent(QPaintEvent * event);
};

#endif // DDESKTOPMENU_H
