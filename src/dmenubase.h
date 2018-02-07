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

#ifndef DMENUBASE_H
#define DMENUBASE_H

// this event type was added in libxcb 1.10,
// but we support also older version
#ifndef XCB_GE_GENERIC
#define XCB_GE_GENERIC 35
#endif

#include <QWidget>
#include <QSharedPointer>
#include <QGraphicsDropShadowEffect>

#include <xcb/xcb.h>
#include <X11/extensions/XI2proto.h>

// Starting from the xcb version 1.9.3 struct xcb_ge_event_t has changed:
// - "pad0" became "extension"
// - "pad1" and "pad" became "pad0"
// New and old version of this struct share the following fields:
typedef struct qt_xcb_ge_event_t {
    uint8_t  response_type;
    uint8_t  extension;
    uint16_t sequence;
    uint32_t length;
    uint16_t event_type;
} qt_xcb_ge_event_t;

class QColor;
class QTimer;
class QMargins;
class QJsonArray;
class QByteArray;
class DMenuContent;
class DMenuBase : public QWidget
{
    Q_OBJECT

public:
    explicit DMenuBase(QWidget *parent = 0);

    struct ItemStyle {
        QColor itemBackgroundColor;
        QColor itemTextColor;
        QColor itemShortcutColor;
        QString checkmarkIcon;
        QString subMenuIndicatorIcon;
    };

    Q_PROPERTY(int radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(QMargins shadowMargins READ shadowMargins WRITE setShadowMargins NOTIFY shadowMarginsChanged)
    Q_PROPERTY(QMargins menuContentMargins READ menuContentMargins WRITE setMenuContentMargins NOTIFY menuContentMarginsChanged)
    Q_PROPERTY(int itemLeftSpacing READ itemLeftSpacing WRITE setItemLeftSpacing NOTIFY itemLeftSpacingChanged)
    Q_PROPERTY(int itemCenterSpacing READ itemCenterSpacing WRITE setItemCenterSpacing NOTIFY itemCenterSpacingChanged)
    Q_PROPERTY(int itemRightSpacing READ itemRightSpacing WRITE setItemRightSpacing NOTIFY itemRightSpacingChanged)

    int radius();
    void setRadius(int);
    QMargins shadowMargins();
    void setShadowMargins(QMargins);
    QMargins menuContentMargins();
    void setMenuContentMargins(QMargins);

    int itemLeftSpacing();
    void setItemLeftSpacing(int);
    int itemCenterSpacing();
    void setItemCenterSpacing(int);
    int itemRightSpacing();
    void setItemRightSpacing(int);

    QSharedPointer<DMenuContent> menuContent();
    void setMenuContent(QSharedPointer<DMenuContent> content);

    DMenuBase *subMenu();

    void setContent(QJsonArray items);
    void destroyAll();
    void grabFocus();
    void releaseFocus();
    DMenuBase *menuUnderPoint(QPoint);
    DMenuBase *getRootMenu();

    void setItemActivity(const QString &itemId, bool isActive);
    void setItemChecked(const QString &itemId, bool checked);
    void setItemText(const QString &itemId, const QString &text);

    const ItemStyle normalStyle();
    const ItemStyle hoverStyle();
    const ItemStyle inactiveStyle();

    virtual void setPosition(int, int) = 0;
    virtual void showSubMenu(int, int, QJsonObject) = 0;

signals:
    void radiusChanged(int radius);
    void shadowMarginsChanged(QMargins shadowWidth);
    void menuContentMarginsChanged(QMargins menuContentMargins);
    void itemBackgroundColorChanged(QColor itemBackgroundColor);
    void itemTextColorChanged(QColor itemTextColor);
    void itemShortcutColorChanged(QColor itemShortcutColor);
    void itemLeftSpacingChanged(int itemLeftSpacing);
    void itemCenterSpacingChanged(int itemCenterSpacing);
    void itemRightSpacingChanged(int itemRightSpacing);

    void itemClicked(QString id, bool checked);

protected:
    DMenuBase *_subMenu;
    ItemStyle _normalStyle;
    ItemStyle _hoverStyle;
    ItemStyle _inactiveStyle;

    virtual bool nativeEvent(const QByteArray &, void *, long *);

private slots:
    void grabFocusSlot();

private:
    int xiErrorBase;
    int xiEventBase;
    int xiOpCode;

    int _radius;
    QMargins _shadowMargins;
    QMargins _menuContentMargins;

    int _itemLeftSpacing;
    int _itemCenterSpacing;
    int _itemRightSpacing;

    QSharedPointer<DMenuContent> _menuContent;
    QGraphicsDropShadowEffect *_dropShadow;
    QTimer *_grabFocusTimer;

    void queryXIExtension();
    bool isXIEvent(xcb_generic_event_t *event, int opCode);
    bool isXIType(xcb_generic_event_t *event, int opCode, uint16_t type);
    qreal fixed1616ToReal(FP1616 val);

    bool grabFocusInternal(int);
    void updateAll();
};

#endif // DMENUBASE_H
