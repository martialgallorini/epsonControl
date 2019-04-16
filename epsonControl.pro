#-------------------------------------------------
#
# Project created by QtCreator 2013-09-03T17:24:34
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = epsonControl
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    projector.cpp \
    pjwidget.cpp

HEADERS  += mainwindow.h \
    projector.h \
    pjwidget.h

FORMS    += mainwindow.ui \
    pjwidget.ui

RESOURCES += \
    img.qrc
