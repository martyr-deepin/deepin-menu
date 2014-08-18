#include <QApplication>

#include "ddesktopmenu.h"
#include "dmenucontent.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DDesktopMenu menu;
    menu.move(100, 100);
    menu.show();

    return a.exec();
}
