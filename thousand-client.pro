#-------------------------------------------------
#
# Project created by QtCreator 2017-04-06T12:04:16
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = thousand-client
TEMPLATE = app


SOURCES += main.cpp\
        game.cpp \
    http_request.cpp

HEADERS  += game.h \
    http_request.h
