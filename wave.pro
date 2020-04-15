QT       += core gui
QT       += sql
QT       += charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
#IF USE C COMPILER
#QMAKE_CFLAGS += -march=armv4t
#IF USE C++ COMPILER
#QMAKE_CXXFLAGS += -march=armv4t


BEFORE_LINK_CMD_LINE = echo Start Build!
QMAKE_PRE_LINK += $$quote($$BEFORE_LINK_CMD_LINE)
AFTER_LINK_CMD_LINE = echo Build Success!
AFTER_LINK_CMD_LINE_2 = &&mv /home/aron566/QtProject/build-wave-NUC972_Cross_Compiler-Release/wave /home/aron566/nfs/
QMAKE_POST_LINK += $$quote($$AFTER_LINK_CMD_LINE)
QMAKE_POST_LINK += $$quote($$AFTER_LINK_CMD_LINE_2)

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += USE_DEFAULT_FRONT_KEYLAYOUT=1
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
include(driver/driver.pri)
include(thread/thread.pri)
static{
QTPLUGIN+=qtvirtualkeyboardplugin
QT+=svg
}
SOURCES += \
    company.cpp \
    history.cpp \
    main.cpp \
    mykeyboard.cpp \
    parameter.cpp \
    parameter2.cpp \
    wavewindow.cpp \
    widget.cpp

HEADERS += \
    company.h \
    history.h \
    mykeyboard.h \
    parameter.h \
    parameter2.h \
    wavewindow.h \
    widget.h

FORMS += \
    company.ui \
    history.ui \
    mykeyboard.ui \
    parameter.ui \
    parameter2.ui \
    wavewindow.ui \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


RESOURCES += \
    BITMAP/BITMAP.qrc

#DISTFILES += \
#    virtualkeyboard.qml
