#ifndef DMENUCONTENT_H
#define DMENUCONTENT_H

#include <QWidget>
#include <QAction>

#include <dmenubase.h>

#define MENU_ITEM_HEIGHT 24
#define MENU_ITEM_FONT_SIZE 14

class QRect;
class DMenuContent : public QWidget
{
    Q_OBJECT
public:
    explicit DMenuContent(DMenuBase *parent = 0);

    int contentWidth();
    int contentHeight();

    int currentIndex();
    void setCurrentIndex(int);

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);

private:
    int _currentIndex;
    QRect getRectOfActionAtIndex(int index);
};

#endif // DMENUCONTENT_H
