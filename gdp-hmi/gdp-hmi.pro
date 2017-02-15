TEMPLATE = app

QT += qml quick
CONFIG += c++11

INCLUDEPATH += HmiController

SOURCES += main.cpp

unix {
    HEADERS += UnixSignalHandler.h
    SOURCES += UnixSignalHandler.cpp
}

desktop {
    DEFINES += DESKTOP_BUILD
}

RESOURCES += assets/assets.qrc \
	     qml/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
