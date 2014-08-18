#ifndef DMENUBASE_H
#define DMENUBASE_H

#include <QWidget>
#include <QGraphicsDropShadowEffect>

class QColor;
class QMargins;
class DMenuBase : public QWidget
{
    Q_OBJECT

public:
    explicit DMenuBase(QWidget *parent = 0);

    enum ItemState {
        NormalState,
        HoverState,
        InactiveState
    };

    Q_PROPERTY(int radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(QMargins shadowMargins READ shadowMargins WRITE setShadowMargins NOTIFY shadowMarginsChanged)
    Q_PROPERTY(QColor itemBackgroundColor READ itemBackgroundColor WRITE setItemBackgroundColor NOTIFY itemBackgroundColorChanged)
    Q_PROPERTY(QColor itemTextColor READ itemTextColor WRITE setItemTextColor NOTIFY itemTextColorChanged)
    Q_PROPERTY(QColor itemShortcutColor READ itemShortcutColor WRITE setItemShortcutColor NOTIFY itemShortcutColorChanged)
    Q_PROPERTY(int itemLeftSpacing READ itemLeftSpacing WRITE setItemLeftSpacing NOTIFY itemLeftSpacingChanged)
    Q_PROPERTY(int itemCenterSpacing READ itemCenterSpacing WRITE setItemCenterSpacing NOTIFY itemCenterSpacingChanged)
    Q_PROPERTY(int itemRightSpacing READ itemRightSpacing WRITE setItemRightSpacing NOTIFY itemRightSpacingChanged)

    int radius();
    void setRadius(int);
    QMargins shadowMargins();
    void setShadowMargins(QMargins);
    QColor itemBackgroundColor();
    void setItemBackgroundColor(QColor);
    QColor itemTextColor();
    void setItemTextColor(QColor);
    QColor itemShortcutColor();
    void setItemShortcutColor(QColor);

    int itemLeftSpacing();
    void setItemLeftSpacing(int);
    int itemCenterSpacing();
    void setItemCenterSpacing(int);
    int itemRightSpacing();
    void setItemRightSpacing(int);

signals:
    void radiusChanged(int radius);
    void shadowMarginsChanged(QMargins shadowWidth);
    void itemBackgroundColorChanged(QColor itemBackgroundColor);
    void itemTextColorChanged(QColor itemTextColor);
    void itemShortcutColorChanged(QColor itemShortcutColor);
    void itemLeftSpacingChanged(int itemLeftSpacing);
    void itemCenterSpacingChanged(int itemCenterSpacing);
    void itemRightSpacingChanged(int itemRightSpacing);

    virtual void setItemState(ItemState) = 0;

private:
    int _radius;
    QMargins _shadowMargins;

    QColor _itemBackgroundColor;
    QColor _itemTextColor;
    QColor _itemShortcutColor;
    int _itemLeftSpacing;
    int _itemCenterSpacing;
    int _itemRightSpacing;

    QGraphicsDropShadowEffect *_dropShadow;
};

#endif // DMENUBASE_H
