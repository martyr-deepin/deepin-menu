#include <QMargins>
#include <QtGlobal>
#include <QGraphicsDropShadowEffect>

#include "dmenubase.h"

DMenuBase::DMenuBase(QWidget *parent) :
    QWidget(parent, Qt::Popup),
    _radius(4),
    _shadowMargins(QMargins(0, 0, 0, 0))
{
    this->setAttribute(Qt::WA_TranslucentBackground);

    _dropShadow = new QGraphicsDropShadowEffect(this);
    _dropShadow->setBlurRadius(0);
    _dropShadow->setColor(Qt::black);
    _dropShadow->setOffset(0.0);
    this->setGraphicsEffect(_dropShadow);
}

// getters and setters
int DMenuBase::radius()
{
    return _radius;
}

void DMenuBase::setRadius(int radius)
{
    if (_radius != radius) {
        _radius = radius;

        emit radiusChanged(radius);
    }
}

QMargins DMenuBase::shadowMargins()
{
    return _shadowMargins;
}

void DMenuBase::setShadowMargins(QMargins shadowMargins)
{
    if (_shadowMargins != shadowMargins) {
        _shadowMargins = shadowMargins;
        _dropShadow->setBlurRadius(qMax(qMax(_shadowMargins.left(), _shadowMargins.top()),
                                        qMax(_shadowMargins.right(), _shadowMargins.bottom())));

        emit shadowMarginsChanged(shadowMargins);
    }
}
