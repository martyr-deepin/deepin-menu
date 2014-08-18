#ifndef DDESKTOPMENU_H
#define DDESKTOPMENU_H

#include <dmenubase.h>

class DDesktopMenu : public DMenuBase
{
public:
    DDesktopMenu();

    void setItemState(ItemState);

protected:
    void paintEvent(QPaintEvent * event);
};

#endif // DDESKTOPMENU_H
