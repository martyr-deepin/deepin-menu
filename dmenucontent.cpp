/**
 * Copyright (C) 2015 Deepin Technology Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 **/

#include <QRect>
#include <QPainter>
#include <QBrush>
#include <QAction>
#include <QtGlobal>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QIcon>
#include <QColor>
#include <QJsonObject>
#include <QJsonArray>
#include <QPoint>
#include <QTextDocument>
#include <QDebug>

#include "utils.h"
#include "dmenubase.h"
#include "dmenucontent.h"

#define MENU_ITEM_MAX_WIDTH 300
#define MENU_ITEM_HEIGHT 24
#define MENU_ITEM_FONT_SIZE 13
#define MENU_ICON_WIDTH 10
#define MENU_ICON_HEIGHT 9
#define SUB_MENU_INDICATOR_WIDTH 6
#define SUB_MENU_INDICATOR_HEIGHT 10
#define SEPARATOR_HEIGHT 6

DMenuContent::DMenuContent(DMenuBase *parent) :
    QWidget(parent),
    _currentIndex(-1)
{
    this->setMouseTracking(true);
}

int DMenuContent::currentIndex()
{
    return _currentIndex;
}

void DMenuContent::setCurrentIndex(int index)
{
    if (index < 0 || _currentIndex == index) return;

    _currentIndex = index;
    this->update();

    if (index < 0 || index >= this->actions().count()) return;

    DMenuBase *parent = qobject_cast<DMenuBase*>(this->parent());
    Q_ASSERT(parent);

    QAction *action = this->actions().at(index);
    QRect actionRect = this->getRectOfActionAtIndex(index);
    QPoint point = this->mapToGlobal(QPoint(this->width(), actionRect.y()));
    QString itemId = action->property("itemId").toString();

    QString prop("%1Active");
    QVariant activeCache = parent->getRootMenu()->property(prop.arg(itemId).toLatin1());
    bool active = activeCache.isNull() ? action->isEnabled() : activeCache.toBool();

    QJsonObject subMenuJsonObj = active ?
                this->actions().at(index)->property("itemSubMenu").value<QJsonObject>()
              : QJsonObject();
    parent->showSubMenu(point.x(), point.y(), subMenuJsonObj);
}

int DMenuContent::contentWidth()
{
    int result = 0;
    _iconWidth = 0;
    _shortcutWidth = 0;
    _subMenuIndicatorWidth = 0;
    DMenuBase *parent = qobject_cast<DMenuBase*>(this->parent());

    QFont font;
    font.setPixelSize(MENU_ITEM_FONT_SIZE);
    QFontMetrics metrics(font);
    foreach (QAction *action, this->actions()) {
        /*
        if(!action->property("itemIcon").toString().isEmpty() || \
           !action->property("itemIconHover").toString().isEmpty() || \
           !action->property("itemIconInactive").toString().isEmpty() || \
           action->isCheckable()) {
            _iconWidth = MENU_ICON_SIZE + parent->itemLeftSpacing();
        }
        */
        _iconWidth = MENU_ICON_WIDTH + parent->itemLeftSpacing();
        if(!action->shortcut().isEmpty()) _shortcutWidth = qMax(_shortcutWidth, metrics.width(action->shortcut().toString()) + parent->itemCenterSpacing());
        //        bool hasSubMenu = action->property("itemSubMenu").value<QJsonObject>()["items"].toArray().count() != 0;
        //        if(hasSubMenu) _subMenuIndicatorWidth = SUB_MENU_INDICATOR_SIZE + parent->itemRightSpacing();
        _subMenuIndicatorWidth = SUB_MENU_INDICATOR_WIDTH + parent->itemRightSpacing();

        result = qMax(result, metrics.width(trimTags(action->text())) + parent->itemCenterSpacing());
    }

    return qMin(MENU_ITEM_MAX_WIDTH,
                result + _iconWidth + _shortcutWidth + _subMenuIndicatorWidth);
}

int DMenuContent::contentHeight()
{
    int result = 0;

    foreach (QAction *action, this->actions()) {
        if (action->text().isEmpty()) {
            result += SEPARATOR_HEIGHT;
        } else {
            result += MENU_ITEM_HEIGHT;
        }
    }

    return result;
}

void DMenuContent::doCurrentAction()
{
    if (_currentIndex < 0 || _currentIndex >= this->actions().count()) return;

    DMenuBase *parent = qobject_cast<DMenuBase*>(this->parent());
    Q_ASSERT(parent);

    QAction *currentAction = this->actions().at(_currentIndex);
    QString itemId = currentAction->property("itemId").toString();
    QString prop("%1Checked");
    QVariant checkedCache = parent->getRootMenu()->property(prop.arg(itemId).toLatin1());
    bool checked = checkedCache.isNull() ? currentAction->isChecked() : checkedCache.toBool();
    bool currentActionHasSubMenu = currentAction->property("itemSubMenu").value<QJsonObject>()["items"].toArray().count() != 0;
    prop = QString("%1Activity");
    QVariant activeCache = parent->getRootMenu()->property(prop.arg(itemId).toLatin1());
    bool active = activeCache.isNull() ? currentAction->isEnabled() : activeCache.toBool();

    if (!active || currentAction->text().isEmpty() || currentActionHasSubMenu) return;

    if (currentAction->isCheckable()) {
        if (checked) {
            this->doUnCheck(_currentIndex);
        } else {
            this->doCheck(_currentIndex);
        }
    } else {
        parent->releaseFocus();
        this->sendItemClickedSignal(currentAction->property("itemId").toString(), false);
    }
    parent->destroyAll();
}

// override methods
void DMenuContent::paintEvent(QPaintEvent *)
{
    DMenuBase *parent = qobject_cast<DMenuBase*>(this->parent());
    QFont font;
    font.setPixelSize(MENU_ITEM_FONT_SIZE);
    QPainter painter(this);
    painter.setFont(font);

    for(int i = 0; i < this->actions().count(); i++) {
        QAction *action = this->actions().at(i);
        QRect actionRect = this->getRectOfActionAtIndex(i);
        QRect actionRectWidthMargins = actionRect.marginsRemoved(QMargins(this->contentsMargins().left(), 0, this->contentsMargins().right(), 0));
        QString itemId = action->property("itemId").toString();

        QString prop("%1Activity");
        QVariant activeCache = parent->getRootMenu()->property(prop.arg(itemId).toLatin1());
        bool active = activeCache.isNull() ? action->isEnabled() : activeCache.toBool();
        DMenuBase::ItemStyle itemStyle = active ? i == _currentIndex ? parent->hoverStyle() : parent->normalStyle() : parent->inactiveStyle();

        // indicates that this item is a separator
        if (action->text().isEmpty()) {
            int topLineX1 = actionRect.x() + 4;
            int topLineY1 = actionRect.y() + (actionRect.height() - 2) / 2;
            int topLineX2 = actionRect.x() + actionRect.width() - 4;
            int topLineY2 = actionRect.y() + (actionRect.height() - 2) / 2;
            int bottomLineX1 = topLineX1;
            int bottomLineY1 = topLineY1 + 1;
            int bottomLineX2 = topLineX2;
            int bottomLineY2 = topLineY1 + 1;
            painter.setPen(QPen(QColor::fromRgbF(0, 0, 0, 0.1)));
            painter.drawLine(topLineX1, topLineY1, topLineX2, topLineY2);
            painter.setPen(QPen(QColor::fromRgbF(1, 1, 1, 0.1)));
            painter.drawLine(bottomLineX1, bottomLineY1, bottomLineX2, bottomLineY2);
        } else {
            painter.fillRect(actionRect, QBrush(itemStyle.itemBackgroundColor));
            painter.setPen(QPen(itemStyle.itemTextColor));

            // draw icon
            if(_iconWidth) {
                QRect iconRect = QRect(actionRectWidthMargins.x() + 4,
                                       actionRectWidthMargins.y() + (MENU_ITEM_HEIGHT - MENU_ICON_HEIGHT) / 2,
                                       MENU_ICON_WIDTH, MENU_ICON_HEIGHT);
                QString prop("%1Checked");
                QVariant checkedCache = parent->getRootMenu()->property(prop.arg(itemId).toLatin1());
                bool checked = checkedCache.isNull() ? action->isChecked() : checkedCache.toBool();

                if (!action->property("itemIcon").toString().isEmpty() || \
                        !action->property("itemIconHover").toString().isEmpty() || \
                        !action->property("itemIconInactive").toString().isEmpty())
                {
                    qDebug() << "draw icon" << iconRect;
                    if (!active) {
                        painter.drawImage(iconRect, QImage(action->property("itemIconInactive").toString()));
                    } else if (_currentIndex == i) {
                        painter.drawImage(iconRect, QImage(action->property("itemIconHover").toString()));
                    } else {
                        painter.drawImage(iconRect, QImage(action->property("itemIcon").toString()));
                    }
                } else if (action->isCheckable() && checked) {
                    qDebug() << "draw checkmark";
                    painter.drawImage(iconRect, QImage(itemStyle.checkmarkIcon));
                }
            }

            // draw text
            QString prop("%1Text");
            QVariant textCache = parent->getRootMenu()->property(prop.arg(itemId).toLatin1());
            QString text = textCache.isNull() ? action->text() : textCache.toString();
            // FIXME(hualet): don't trust the magic +4 operation, I don't know why
            // we should +4. All these crap started when I introduced QTextDocument...
            QString elidedText = elideText(text, actionRectWidthMargins.width() + 4);

            painter.save();
            // move the start point to the right place.
            // FIXME: don't know why we should do the -4 operation.
            painter.translate(actionRectWidthMargins.x() + _iconWidth - 4,
                              actionRectWidthMargins.y());
            if (font.defaultFamily() == "Source Han Sans SC") {
                painter.translate(0, -4);
            }

            QString itemRichText = QString("<font color='%1'>%2</font>") \
                                            .arg(painter.pen().color().name()) \
                                            .arg(elidedText);

            // no help.
//            QTextOption opt;
//            opt.setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

            QTextDocument doc;
            doc.setHtml(itemRichText);
            doc.setDefaultFont(font);
//            doc.setDefaultTextOption(opt);

            doc.drawContents(&painter);
            painter.restore();

            // draw shortcut text
            if(_shortcutWidth)
                painter.drawText(QRect(actionRectWidthMargins.x(),
                                       actionRectWidthMargins.y(),
                                       actionRectWidthMargins.width() - _subMenuIndicatorWidth,
                                       actionRectWidthMargins.height()),
                                 Qt::AlignVCenter | Qt::AlignRight,
                                 action->shortcut().toString());

            // draw sub menu indicator icon
            if(action->property("itemSubMenu").value<QJsonObject>()["items"].toArray().count() != 0)
                painter.drawImage(QRect(actionRectWidthMargins.x() + actionRectWidthMargins.width() - SUB_MENU_INDICATOR_WIDTH,
                                        actionRectWidthMargins.y() + (MENU_ITEM_HEIGHT - SUB_MENU_INDICATOR_HEIGHT) / 2,
                                        SUB_MENU_INDICATOR_WIDTH, SUB_MENU_INDICATOR_HEIGHT),
                                  QImage(itemStyle.subMenuIndicatorIcon));
        }
    }

    painter.end();
}

void DMenuContent::mouseMoveEvent(QMouseEvent *event)
{
    int index = itemIndexUnderEvent(event);
    setCurrentIndex(index);
}

void DMenuContent::keyPressEvent(QKeyEvent *event)
{
    DMenuBase *parent = qobject_cast<DMenuBase*>(this->parent());
    Q_ASSERT(parent);

    switch (event->key()) {
    case Qt::Key_Escape:
        parent->destroyAll();
        break;
    case Qt::Key_Return:
        this->doCurrentAction();
        break;
    case Qt::Key_Up:
        this->selectPrevious();
        break;
    case Qt::Key_Down:
        this->selectNext();
        break;
    case Qt::Key_Right:
        if (parent->subMenu() && parent->subMenu()->isVisible())
            parent->subMenu()->grabFocus();
        break;
    case Qt::Key_Left:
        if (parent->parent()) {
            DMenuBase *p_parent = qobject_cast<DMenuBase*>(parent->parent());
            Q_ASSERT(p_parent);

            p_parent->grabFocus();
        }
        break;
    }
}

// private methods
QRect DMenuContent::getRectOfActionAtIndex(int index)
{
    int previousHeight = 0;
    int itemHeight = this->actions().at(index)->text().isEmpty() ? SEPARATOR_HEIGHT : MENU_ITEM_HEIGHT;

    for (int i = 0; i < index; i++) {
        QAction *action = this->actions().at(i);
        if (action->text().isEmpty()) {
            previousHeight += SEPARATOR_HEIGHT;
        } else {
            previousHeight += MENU_ITEM_HEIGHT;
        }
    }

    return QRect(0, previousHeight, this->width(), itemHeight);
}

void DMenuContent::clearActions()
{
    foreach (QAction *action, this->actions()) {
        this->removeAction(action);
    }
}

int DMenuContent::getNextItemsHasShortcut(int startPos, QString keyText) {
    if (keyText.isEmpty()) return -1;

    DMenuBase *parent = qobject_cast<DMenuBase*>(this->parent());
    Q_ASSERT(parent);

    for (int i = qMax(startPos, 0); i < this->actions().count(); i++) {
        QString itemId(this->actions().at(i)->property("itemId").toString());
        QAction *action = this->actions().at(i);

        QString prop("%1Active");
        QVariant activeCache = parent->getRootMenu()->property(prop.arg(itemId).toLatin1());
        bool active = activeCache.isNull() ? action->isEnabled() : activeCache.toBool();

        // a trick here, using currentIndex as the cursor.
        if (active && keyText == this->actions().at(i)->property("itemNavKey").toString().toLower()){
            return i;
        }
    }

    // we do the check another time to support wrapping.
    for (int i = 0; i < this->actions().count(); i++) {
        QString itemId(this->actions().at(i)->property("itemId").toString());
        QAction *action = this->actions().at(i);

        QString prop("%1Active");
        QVariant activeCache = parent->getRootMenu()->property(prop.arg(itemId).toLatin1());
        bool active = activeCache.isNull() ? action->isEnabled() : activeCache.toBool();

        if (active && keyText == this->actions().at(i)->property("itemNavKey").toString().toLower()) {
            return i;
        }
    }
    return -1;
}

void DMenuContent::selectPrevious()
{
    DMenuBase *parent = qobject_cast<DMenuBase*>(this->parent());
    Q_ASSERT(parent);

    for (int i = currentIndex() - 1; i >= 0; i--) {
        QAction * action = actions().at(i);

        QString itemId(action->property("itemId").toString());

        QString prop("%1Active");
        QVariant activeCache = parent->getRootMenu()->property(prop.arg(itemId).toLatin1());
        bool active = activeCache.isNull() ? action->isEnabled() : activeCache.toBool();

        if (active && !action->text().isEmpty()) {
            setCurrentIndex(i);
            break;
        }
    }
}

void DMenuContent::selectNext()
{
    DMenuBase *parent = qobject_cast<DMenuBase*>(this->parent());
    Q_ASSERT(parent);

    for (int i = currentIndex() + 1; i < actions().count(); i++) {
        QAction * action = actions().at(i);

        QString itemId(action->property("itemId").toString());

        QString prop("%1Active");
        QVariant activeCache = parent->getRootMenu()->property(prop.arg(itemId).toLatin1());
        bool active = activeCache.isNull() ? action->isEnabled() : activeCache.toBool();

        if (active && !action->text().isEmpty()) {
            setCurrentIndex(i);
            break;
        }
    }
}

void DMenuContent::doCheck(int index) {
    DMenuBase *parent = qobject_cast<DMenuBase*>(this->parent());
    Q_ASSERT(parent);

    QAction *action = this->actions().at(index);
    QString itemId = action->property("itemId").toString();
    parent->getRootMenu()->setItemChecked(itemId, true);
    this->sendItemClickedSignal(itemId, true);

    if (Utils::menuItemCheckableFromId(itemId)) {
        QStringList components = itemId.split(':');
        QString group = components.at(0);

        foreach (QAction *act, this->actions()) {
            QString _itemId = act->property("itemId").toString();

            if (act != action && Utils::menuItemCheckableFromId(_itemId)) {
                QStringList _components = _itemId.split(':');
                QString _group = _components.at(0);
                QString _type = _components.at(1);

                if (_type == "radio" && _group == group) {
                    parent->getRootMenu()->setItemChecked(_itemId, false);
                }
            }
        }
    }

    this->update();
}

void DMenuContent::doUnCheck(int index)
{
    DMenuBase *parent = qobject_cast<DMenuBase*>(this->parent());
    Q_ASSERT(parent);

    QAction *action = this->actions().at(index);
    QString itemId = action->property("itemId").toString();

    if (Utils::menuItemCheckableFromId(itemId)) {
        QStringList components = itemId.split(':');
        QString group = components.at(0);
        QString type = components.at(1);

        if (type == "radio") {
            bool hasNoCheck = true;

            foreach (QAction *act, this->actions()) {
                QString _itemId = act->property("itemId").toString();

                if (Utils::menuItemCheckableFromId(_itemId) && _itemId != itemId) {
                    QStringList components = _itemId.split(':');
                    QString _group = components.at(0);
                    QString _type = components.at(1);

                    if (group == _group && _type == "radio") {
                        QString prop("%1Checked");
                        QVariant checkedCache = parent->getRootMenu()->property(prop.arg(_itemId).toLatin1());
                        bool checked = checkedCache.isNull() ? act->isChecked() : checkedCache.toBool();

                        hasNoCheck = hasNoCheck && !checked;
                    }
                }
            }

            if (!hasNoCheck) {
                parent->getRootMenu()->setItemChecked(itemId, false);
                this->sendItemClickedSignal(action->property("itemId").toString(), false);
            } else {
                parent->getRootMenu()->setItemChecked(itemId, true);
                this->sendItemClickedSignal(action->property("itemId").toString(), true);
            }
        } else {
            parent->getRootMenu()->setItemChecked(itemId, false);
            this->sendItemClickedSignal(action->property("itemId").toString(), false);
        }
    } else {
        parent->getRootMenu()->setItemChecked(itemId, false);
        this->sendItemClickedSignal(action->property("itemId").toString(), false);
    }

    this->update();
}

void DMenuContent::sendItemClickedSignal(QString id, bool checked)
{
    DMenuBase *root = qobject_cast<DMenuBase *>(this->parent());
    Q_ASSERT(root);
    while (root) {
        if (!root->parent()) {
            root->itemClicked(id, checked);
            break;
        } else {
            root = qobject_cast<DMenuBase *>(root->parent());
            Q_ASSERT(root);
        }
    }
}

int DMenuContent::itemIndexUnderEvent(QMouseEvent *event) const
{
    DMenuBase *parent = qobject_cast<DMenuBase*>(this->parent());
    Q_ASSERT(parent);

    DMenuBase *menuUnderCursor = parent->menuUnderPoint(event->globalPos());
    if (menuUnderCursor == parent) {
        int previousHeight = this->rect().y();

        for (int i = 0; i < this->actions().count(); i++) {
            QAction *action = this->actions().at(i);
            int itemHeight = action->text().isEmpty() ? SEPARATOR_HEIGHT : MENU_ITEM_HEIGHT;

            if (previousHeight <= event->y() &&
                    event->y() <= previousHeight + itemHeight &&
                    this->rect().x() <= event->x() &&
                    event->x() <= this->rect().x() + this->rect().width()) {
                return i;
            } else {
                previousHeight += itemHeight;
            }
        }
    }

    return -1;
}

QString DMenuContent::elideText(QString source, int maxWidth) const
{
    QFont font;
    font.setPixelSize(MENU_ITEM_FONT_SIZE);
    QFontMetrics metrics(font);

    if (metrics.width(source) < maxWidth) {
        return source;
    } else {
        return metrics.elidedText(source, Qt::ElideRight, maxWidth);
    }
}

QString DMenuContent::trimTags(QString source) const
{
    QString result(source);
    result.replace("<u>", "");
    result.replace("</u>", "");

    return result;
}
