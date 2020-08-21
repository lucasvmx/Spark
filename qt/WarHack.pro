#-------------------------------------------------
#
# Project created by QtCreator 2018-01-15T23:56:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WarHack
TEMPLATE = app
win32:RC_ICONS += ../resource/images/warzone2100_icon.ico

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

Debug:DEFINES += _DEBUG
Release:DEFINES += _RELEASE

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        ../src/main.cpp \
        ../src/frmmain.cpp \
    ../src/wzhack.cpp \
    ../src/frmsettings.cpp \
    ../src/task.cpp \
	../src/frmabout.cpp

HEADERS += \
	../libs/libhack/consts.h \
	../libs/libhack/hack.h \
	../libs/libhack/process.h \
        ../src/frmmain.h \
	../src/version.h \
    ../src/wzhack.h \
    ../src/frmsettings.h \
    ../src/task.h \
	../src/frmabout.h

FORMS += \
        ../ui/frmmain.ui \
    ../ui/frmsettings.ui \
    ../ui/frmabout.ui

RESOURCES += \
    ../resource/resources.qrc

win32: LIBS += -L$$PWD/../libs/libhack -lhack -lpsapi

INCLUDEPATH += $$PWD/../libs/libhack
DEPENDPATH += $$PWD/../libs/libhack
