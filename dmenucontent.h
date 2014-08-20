#ifndef DMENUCONTENT_H
#define DMENUCONTENT_H

#include <QWidget>
#include <QAction>

#include <dmenubase.h>

#define MENU_ITEM_HEIGHT 24
#define MENU_ITEM_FONT_SIZE 12
#define MENU_ICON_SIZE 14
#define SUB_MENU_INDICATOR_SIZE 14
#define SEPARATOR_HEIGHT 6

class QRect;
class DMenuBase;
class DMenuContent : public QWidget
{
    Q_OBJECT
public:
    explicit DMenuContent(DMenuBase *parent = 0);

    int contentWidth();
    int contentHeight();

    int currentIndex();
    void setCurrentIndex(int);

    void clearActions();
    void doCurrentAction();

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

private:
    int _iconWidth;
    int _shortcutWidth;
    int _subMenuIndicatorWidth;

    int _currentIndex;
    QRect getRectOfActionAtIndex(int);
    int getNextItemsHasShortcut(int, QString);
    void doCheck(int);
    void doUnCheck(int);
    void sendItemClickedSignal(QString, bool);
};

#endif // DMENUCONTENT_H
