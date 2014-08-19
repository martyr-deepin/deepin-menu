#ifndef DMENUBASE_H
#define DMENUBASE_H

#include <QWidget>
#include <QSharedPointer>
#include <QGraphicsDropShadowEffect>

class QColor;
class QMargins;
class QJsonArray;
class DMenuContent;
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
    Q_PROPERTY(QMargins menuContentMargins READ menuContentMargins WRITE setMenuContentMargins NOTIFY menuContentMarginsChanged)
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
    QMargins menuContentMargins();
    void setMenuContentMargins(QMargins);
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

    QSharedPointer<DMenuContent> menuContent();
    void setMenuContent(QSharedPointer<DMenuContent> content);

    QString checkmarkIcon();
    void setCheckmarkIcon(QString icon);
    QString subMenuIndicatorIcon();
    void setSubMenuIndicatorIcon(QString icon);

    void setContent(QJsonArray items);
    void destroyAll();

    virtual void setItemState(ItemState) = 0;
    virtual void setPosition(int, int) = 0;

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

private:
    int _radius;
    QMargins _shadowMargins;
    QMargins _menuContentMargins;
    QString _checkmarkIcon;
    QString _subMenuIndicatorIcon;

    QColor _itemBackgroundColor;
    QColor _itemTextColor;
    QColor _itemShortcutColor;
    int _itemLeftSpacing;
    int _itemCenterSpacing;
    int _itemRightSpacing;

    QSharedPointer<DMenuContent> _menuContent;
    QGraphicsDropShadowEffect *_dropShadow;

    bool menuItemCheckableFromId(QString);
};

#endif // DMENUBASE_H
