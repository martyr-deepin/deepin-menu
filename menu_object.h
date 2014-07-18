#ifndef MENU_OBJECT_H
#define MENU_OBJECT_H

#include <QObject>

class MenuObject : public QObject
{
    Q_OBJECT
public:
    explicit MenuObject(QObject *parent = 0);

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
};

#endif // MENU_OBJECT_H
