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

class QRect;
class DDockMenu;
class DMenuContent : public QWidget
{
    Q_OBJECT
public:
    explicit DMenuContent(DDockMenu *parent = 0);

    int contentWidth();
    int contentHeight();

    int currentIndex();
    void setCurrentIndex(int);

    void clearActions();
    void doCurrentAction();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    friend class DDockMenu;
    void processCursorMove(int x, int y);
    void processButtonClick(int x, int y);
    void processKeyPress(const QString &key);

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
    int itemIndexUnderEvent(QPoint point) const;
    QString elideText(QString source, int maxWidth) const;
};

#endif // DMENUCONTENT_H
