QT     += core gui sql
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Reminder
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    database.cpp \
    dialogaddtask.cpp \
    tablemodel.cpp \
    popup.cpp \
    dialogsettime.cpp

HEADERS  += mainwindow.h \
    database.h \
    dialogaddtask.h \
    tablemodel.h \
    popup.h \
    dialogsettime.h
