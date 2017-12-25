#-------------------------------------------------
#
# Project created by QtCreator 2017-12-25T15:11:06
#
#-------------------------------------------------

QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Keithley2400
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32  {
    LIBS += -Lvisa
    INCLUDEPATH += "c:/Program Files (x86)/IVI Foundation/VISA/WinNT/include/"
    LIBS += "C:/WINDOWS/SYSTEM32/visa32.dll"
}

SOURCES += \
        main.cpp \
        maindialog.cpp

HEADERS += \
        maindialog.h

FORMS += \
        maindialog.ui
