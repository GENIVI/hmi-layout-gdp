TEMPLATE = lib
TARGET = hmi-controller
QT += qml quick dbus
CONFIG += qt plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)
uri = com.genivi.hmicontroller

# ILM libs
LIBS += -lilmCommon -lilmClient -lilmControl -lilmInput

#Systemd libs
LIBS += -lsystemd

# Input
SOURCES += \
    hmi-controller_plugin.cpp \
    hmicontroller.cpp \
    layercontroller.cpp \
    appmanager.cpp \
    allapplicationsmodel.cpp \
    homeapplicationsmodel.cpp \
    applicationsmodelbase.cpp \
    lifecycleconsumer.cpp \
    lifecycleconsumeradaptor.cpp

HEADERS += \
    hmi-controller_plugin.h \
    hmicontroller.h \
    layercontroller.h \
    appmanager.h \
    ivi-controller-client-protocol.h \
    allapplicationsmodel.h \
    homeapplicationsmodel.h \
    applicationsmodelbase.h \
    lifecycleconsumer.h \
    lifecycleconsumeradaptor.h

DISTFILES = qmldir

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

# Avoid going to release/debug subdirectory
win32: DESTDIR = ./

qmldir.files = qmldir
installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
qmldir.path = $$installPath
target.path = $$installPath
INSTALLS += target qmldir
