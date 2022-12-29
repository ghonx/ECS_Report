#-------------------------------------------------
#
# Project created by QtCreator 2013-04-13T19:01:21
#           version : 1.0
#-------------------------------------------------

VERSION = 1.0.0.0
TARGET = QtEcsLibrary
QT       += core printsupport

SOURCES += qecsreport.cpp \
    sectionclass.cpp \
    layoutcol.cpp
HEADERS += qecsreport.h \
    sectionclass.h \
    layoutcol.h


# ------------ GUI SECTION --------------
TEMPLATE = app
QT += gui widgets sql
SOURCES += main.cpp\
        mainwindow.cpp
HEADERS  +=  mainwindow.h
FORMS    += mainwindow.ui

DESTDIR = $$PWD/bin

RESOURCES += \
    res.qrc
