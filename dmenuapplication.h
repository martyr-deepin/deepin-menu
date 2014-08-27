#ifndef DMENUAPPLICATION_H
#define DMENUAPPLICATION_H

#include <QApplication>

class DMenuApplication : public QApplication
{
    Q_OBJECT
public:
    explicit DMenuApplication(int &, char**);

public slots:
    void quitApplication(const QString&);
};

#endif // DMENUAPPLICATION_H
