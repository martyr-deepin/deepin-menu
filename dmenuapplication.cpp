#include "dmenuapplication.h"

DMenuApplication::DMenuApplication(int &argc, char *argv[]) :
    QApplication(argc, argv)
{
}

void DMenuApplication::quitApplication(const QString &)
{
    this->quit();
}
