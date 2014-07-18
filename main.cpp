#include <QGuiApplication>
#include <QQuickView>
#include <QQuickItem>
#include <QQmlEngine>
#include <QQmlContext>
#include <QColor>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickView menu;
    menu.setFlags(Qt::Tool | Qt::BypassWindowManagerHint);
    menu.setResizeMode(QQuickView::SizeViewToRootObject);
    menu.setColor(QColor(0, 0, 0, 0));
    menu.setSource(QUrl(QStringLiteral("qrc:///FullscreenBackground.qml")));
    menu.show();
    menu.requestActivate();

    QVariant x = 300;
    QVariant y = 300;
    QVariant content =
    QMetaObject::invokeMethod(menu.rootObject(), "showMenu",
            Q_ARG(QVariant, x), Q_ARG(QVariant, y), Q_ARG(QVariant, content));

    QQmlEngine *engine = menu.rootContext()->engine();
    QObject::connect(engine, SIGNAL(quit()), &app, SLOT(quit()));

    return app.exec();
}
