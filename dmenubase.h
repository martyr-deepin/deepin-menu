#ifndef DMENUBASE_H
#define DMENUBASE_H

#include <QWidget>
#include <QSharedPointer>
#include <QGraphicsDropShadowEffect>

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
    int _radius;
    QMargins _shadowMargins;
    QMargins _menuContentMargins;

    int _itemLeftSpacing;
    int _itemCenterSpacing;
    int _itemRightSpacing;

    QSharedPointer<DMenuContent> _menuContent;
    QGraphicsDropShadowEffect *_dropShadow;
    QTimer *_grabFocusTimer;

    bool grabFocusInternal(int);
};

#endif // DMENUBASE_H
