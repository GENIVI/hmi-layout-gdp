#include <QGuiApplication>
#include <QQmlApplicationEngine>

#ifdef Q_OS_UNIX
#include "UnixSignalHandler.h"
#endif // Q_OS_UNIX

// TODO. Why does Qt for RPi3 want to used gles3? Only gles2 is in sysroot.
#undef QT_OPENGL_ES_3
#include <QQuickView>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Only create background surface on device
#ifndef DESKTOP_BUILD
    QQuickView background;
    background.setFlags(Qt::FramelessWindowHint);
    background.setResizeMode(QQuickView::SizeRootObjectToView);
    background.setProperty("IVI-Surface-ID", 0);
    background.setSource(QUrl(QStringLiteral("qrc:/qml/wayland-background.qml")));
    background.showFullScreen();
#endif // NOT DESKTOP_BUILD

    //Launcher
    QQmlApplicationEngine engine;

    // Load Desktop or Device QML main file
#ifdef DESKTOP_BUILD
    engine.load(QUrl(QStringLiteral("qrc:/qml/DesktopMain.qml")));
#else
    engine.load(QUrl(QStringLiteral("qrc:/qml/DeviceMain.qml")));
#endif // DESKTOP_BUILD

    //Gracefully exit on SIGINT from terminal and SIGTERM from systemd on UNIX
#ifdef Q_OS_UNIX
    UnixSignalHandler signalHandler;
    QObject::connect(&signalHandler, &UnixSignalHandler::interruptRequested,
                     &app, &QCoreApplication::quit);
    QObject::connect(&signalHandler, &UnixSignalHandler::terminateRequested,
                     &app, &QCoreApplication::quit);
#endif // Q_OS_UNIX

    return app.exec();
}
