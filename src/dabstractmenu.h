#ifndef DABSTRACTMENU_H
#define DABSTRACTMENU_H

#include <QObject>

class DAbstractMenu
{
public:
    explicit DAbstractMenu();

public:
    virtual void grabFocus();
    virtual void releaseFocus();

    virtual void setItems(QJsonArray items);

    virtual void setItemActivity(const QString &itemId, bool isActive);
    virtual void setItemChecked(const QString &itemId, bool checked);
    virtual void setItemText(const QString &itemId, const QString &text);
};

#endif // DABSTRACTMENU_H
