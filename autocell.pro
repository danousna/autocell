#-------------------------------------------------
#
# Project created by QtCreator 2018-05-19T15:08:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = autocell
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


SOURCES += \
    sources/automate.cpp \
    sources/automateexception.cpp \
    sources/automatemanager.cpp \
    sources/etat.cpp \
    sources/main.cpp \
    sources/mainwindow.cpp \
    sources/simulateur.cpp

HEADERS += \
    headers/automate.h \
    headers/automateexception.h \
    headers/automatemanager.h \
    headers/etat.h \
    headers/mainwindow.h \
    headers/simulateur.h

FORMS += \
        mainwindow.ui
