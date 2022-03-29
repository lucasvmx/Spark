#-------------------------------------------------
#
# Project created by QtCreator 2018-01-15T23:56:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Spark
TEMPLATE = app
win32:RC_ICONS += ../resource/images/iconfinder__snowflake_1679761_ico.ico
win32:RC_FILE += ../resource/spark.rc

VERSION = 2.4.0.0
if(!greaterThan(QT_MAJOR_VERSION, 5)) {
	QMAKE_TARGET_COMPANY = "Lucas Vieira"
	QMAKE_TARGET_DESCRIPTION = "Spark - a simple trainer to warzone 2100"
	QMAKE_TARGET_COPYRIGHT = "Lucas Vieira de Jesus"
	QMAKE_TARGET_PRODUCT = "Spark"
	# Enable exceptions
	QMAKE_CXXFLAGS += -fexceptions
}

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
	../src/dynamic_loader.c \
	../src/exception.cpp \
        ../src/main.cpp \
        ../src/frmmain.cpp \
	../src/threads.cpp \
    ../src/wzhack.cpp \
    ../src/frmsettings.cpp \
	../src/frmabout.cpp \
    ../src/frmlanguage.cpp \
	../src/frmupdate.cpp \
	../src/platform.cpp

TRANSLATIONS += ../languages/spark_en_us.translation

CONFIG += lrelease

HEADERS += \
	../3rdparty/libhack/src/consts.h \
	../3rdparty/libhack/src/init.h \
	../3rdparty/libhack/src/process.h \
	../resource/spark.rc \
	../src/dynamic_loader.h \
	../src/exception.h \
        ../src/frmmain.h \
	../src/threads.h \
	../src/version.h \
    ../src/wzhack.h \
    ../src/frmsettings.h \
	../src/frmabout.h \
    ../src/frmlanguage.h \
	../src/frmupdate.h \
	../src/platform.h

FORMS += \
        ../ui/frmmain.ui \
    ../ui/frmsettings.ui \
    ../ui/frmabout.ui \
    ../ui/frmlanguage.ui \
	../ui/frmupdate.ui

RESOURCES += \
    ../resource/resources.qrc

win32: LIBS += -L$$PWD/../BUILD/ -lhack -lshlwapi -lpsapi -static-libgcc

INCLUDEPATH += $$PWD/../3rdparty/libhack/src/
DEPENDPATH += $$PWD/../BUILD/

DISTFILES += \
	../resource/images/icon_update.ico
