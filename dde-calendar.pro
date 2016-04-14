#-------------------------------------------------
#
# Project created by QtCreator 2016-03-17T15:35:18
#
#-------------------------------------------------

QT       += core gui dbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dde-calendar
TEMPLATE = app
CONFIG += link_pkgconfig c++11
PKGCONFIG += dtkbase dtkwidget

SOURCES += main.cpp\
    calendarview.cpp \
    calendarwindow.cpp \
    calendardbus.cpp \
    calendartitlebarwidget.cpp

HEADERS  += calendarwindow.h \
    calendardbus.h \
    calendarview.h \
    calendartitlebarwidget.h \
    constants.h

RESOURCES += \
    resources.qrc
