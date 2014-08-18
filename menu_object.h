#ifndef MENU_OBJECT_H
#define MENU_OBJECT_H

#include <QObject>

class ManagerObject;
class DDesktopMenu;
class MenuObject : public QObject
{
    Q_OBJECT
public:
    MenuObject(ManagerObject *manager);
    ~MenuObject();

signals:
    void ItemInvoked(const QString &itemId, bool checked);
    void MenuUnregistered();
public slots:
    void SetItemActivity(const QString &itemId, bool isActive);
    void SetItemChecked(const QString &itemId, bool checked);
    void SetItemText(const QString &itemId, const QString &text);
    void ShowMenu(const QString &menuJsonContent);

    void destroyMenu();

private:
    DDesktopMenu *menu;
    ManagerObject *manager;
};

#endif // MENU_OBJECT_H
