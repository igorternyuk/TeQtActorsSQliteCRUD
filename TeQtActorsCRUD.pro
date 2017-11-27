#-------------------------------------------------
#
# Project created by QtCreator 2017-11-26T18:36:08
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++1z
DEFINES += DEBUG
TARGET = TeQtActorsCRUD
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    dialogcountry.cpp \
    db_manager.cpp \
    actor.cpp \
    dialoginsertactor.cpp \
    comboboxsqlmodel.cpp

HEADERS  += mainwindow.h \
    dialogcountry.h \
    db_manager.hpp \
    actor.hpp \
    dialoginsertactor.hpp \
    comboboxsqlmodel.hpp

FORMS    += mainwindow.ui \
    dialogcountry.ui \
    dialoginsertactor.ui

RESOURCES += \
    resources.qrc
