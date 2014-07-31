#ifndef MENU_OBJECT_H
#define MENU_OBJECT_H

#include <QObject>
#include <QQuickView>

class ManagerObject;

class MenuObject : public QObject
{
    Q_OBJECT
public:
    MenuObject(ManagerObject *manager);
    ~MenuObject();

signals:
    void ItemActivitySet(const QString &itemId, bool isActive);
    void ItemCheckedSet(const QString &itemId, bool checked);
    void ItemInvoked(const QString &itemId, bool checked);
    void ItemTextSet(const QString &itemId, const QString &text);
    void MenuUnregistered();
public slots:
    void SetItemActivity(const QString &itemId, bool isActive);
    void SetItemChecked(const QString &itemId, bool checked);
    void SetItemText(const QString &itemId, const QString &text);
    void ShowMenu(const QString &menuJsonContent);

    void destroyMenu();

private:
    QQuickView *menu;
    ManagerObject *manager;
};

#endif // MENU_OBJECT_H
