#ifndef DDESKTOPMENU_H
#define DDESKTOPMENU_H

#include <dmenubase.h>

class QJsonObject;
class DDesktopMenu : public DMenuBase
{
public:
    DDesktopMenu(DDesktopMenu *parent=0);

    virtual void setItemState(ItemState);
    virtual void setPosition(int, int);

protected:
    virtual void paintEvent(QPaintEvent * event);

private:
    virtual void showSubMenu(int, int, QJsonObject);
};

#endif // DDESKTOPMENU_H
