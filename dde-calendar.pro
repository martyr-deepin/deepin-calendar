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
    calendartitlebarwidget.cpp \
    dbuscalendar_adaptor.cpp

HEADERS  += calendarwindow.h \
    calendardbus.h \
    calendarview.h \
    calendartitlebarwidget.h \
    constants.h \
    dbuscalendar_adaptor.h

RESOURCES += \
    resources.qrc

target.path = /usr/bin/

desktop_files.files = deepin-calendar.desktop
desktop_files.path = /usr/share/applications/

icon_files.files = resources/icon/deepin-calendar.svg
icon_files.path = /usr/share/icons/hicolor/scalable/apps/

INSTALLS += target desktop_files icon_files
