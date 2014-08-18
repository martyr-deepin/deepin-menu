#ifndef DMENUBASE_H
#define DMENUBASE_H

#include <QWidget>
#include <QGraphicsDropShadowEffect>

class QMargins;
class DMenuBase : public QWidget
{
    Q_OBJECT

public:
    explicit DMenuBase(QWidget *parent = 0);

    Q_PROPERTY(int radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(QMargins shadowMargins READ shadowMargins WRITE setShadowMargins NOTIFY shadowMarginsChanged)

    int radius();
    void setRadius(int);
    QMargins shadowMargins();
    void setShadowMargins(QMargins);

signals:
    void radiusChanged(int radius);
    void shadowMarginsChanged(QMargins shadowWidth);

private:
    int _radius;
    QMargins _shadowMargins;

    QGraphicsDropShadowEffect *_dropShadow;
};

#endif // DMENUBASE_H
