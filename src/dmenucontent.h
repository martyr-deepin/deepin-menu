/*
 * Copyright (C) 2015 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     Hualet Wang <mr.asianwang@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
    void processCursorMove(const QPoint &p);
    void processButtonClick(const QPoint &p);

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
