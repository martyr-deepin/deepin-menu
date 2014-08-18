#ifndef DMENUCONTENT_H
#define DMENUCONTENT_H

#include <QWidget>

class DMenuContent : public QWidget
{
    Q_OBJECT
public:
    explicit DMenuContent(QWidget *parent = 0);

    int contentWidth();
    int contentHeight();

protected:
    void paintEvent(QPaintEvent * event);
};

#endif // DMENUCONTENT_H
