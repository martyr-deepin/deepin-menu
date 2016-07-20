/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#ifndef DMENUCONTENT_H
#define DMENUCONTENT_H

#include <QWidget>
#include <QAction>

#include <dmenubase.h>

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
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);

private:
    int _iconWidth;
    int _shortcutWidth;
    int _subMenuIndicatorWidth;

    int _currentIndex;
    QRect getRectOfActionAtIndex(int);
    int getNextItemsHasShortcut(int, QString);
    void selectPrevious();
    void selectNext();
    void doCheck(int);
    void doUnCheck(int);
    void sendItemClickedSignal(QString, bool);
    int itemIndexUnderEvent(QMouseEvent *event) const;
    QString elideText(QString source, int maxWidth) const;
    QString trimTags(QString source) const;
};

#endif // DMENUCONTENT_H
