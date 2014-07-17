#ifndef MENU_H
#define MENU_H

#include <QQuickView>

class Menu : public QQuickView
{
public:
    Menu *parentMenu;
    Menu *childMenu;

    Menu();
    ~Menu();
};

#endif // MENU_H
