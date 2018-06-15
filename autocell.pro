#-------------------------------------------------
#
# Project created by QtCreator 2018-05-19T15:08:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoCell
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
    src/automate.cpp \
    src/etat.cpp \
    src/simulateur.cpp \
    src/main.cpp \
    src/ui/mainwindow.cpp \
    src/automate1d.cpp \
    src/cell.cpp \
    src/grille.cpp \
    src/automate2d.cpp \
    src/autocellexception.cpp \
    src/ui/elementaireview.cpp \
    src/ui/golview.cpp \
    src/ui/wwview.cpp

HEADERS += \
    src/automate.h \
    src/etat.h \
    src/simulateur.h \
    src/ui/mainwindow.h \
    src/automate1d.h \
    src/cell.h \
    src/grille.h \
    src/automate2d.h \
    src/autocellexception.h \
    src/ui/elementaireview.h \
    src/ui/golview.h \
    src/ui/wwview.h

FORMS += \
    src/ui/mainwindow.ui \
    src/ui/elementaireview.ui \
    src/ui/golview.ui \
    src/ui/wwview.ui
