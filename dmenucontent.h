#ifndef DMENUCONTENT_H
#define DMENUCONTENT_H

#include <QWidget>
#include <QAction>

#define MENU_ITEM_HEIGHT 24
#define MENU_ITEM_FONT_SIZE 14

class QRect;
class DMenuContent : public QWidget
{
    Q_OBJECT
public:
    explicit DMenuContent(QWidget *parent = 0);

    int contentWidth();
    int contentHeight();

protected:
    void paintEvent(QPaintEvent * event);

private:
    QRect getRectOfActionAtIndex(int index);
};

#endif // DMENUCONTENT_H
