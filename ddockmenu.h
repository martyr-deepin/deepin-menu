#ifndef DDOCKMENU_H
#define DDOCKMENU_H

#include "dmenubase.h"

#define CORNER_WIDTH 18
#define CORNER_HEIGHT 10

class DDockMenu : public DMenuBase
{
public:
    DDockMenu(DDockMenu *parent = 0);

    virtual void setPosition(int, int);

protected:
    virtual void paintEvent(QPaintEvent *);

private:
    int m_cornerX;
    virtual void showSubMenu(int, int, QJsonObject);

    void moveCornerX(int);
};

#endif // DDOCKMENU_H
