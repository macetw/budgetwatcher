QT += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = budgetwatcher
TEMPLATE = app


SOURCES += budgethistorymodel.cpp \
           main.cpp \
           mainwindow.cpp
 

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

CONFIG += mobility
MOBILITY +=
