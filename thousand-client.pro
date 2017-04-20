#-------------------------------------------------
#
# Project created by QtCreator 2017-04-06T12:04:16
#
#-------------------------------------------------

QT       += core gui network
CONFIG   += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = thousand-client
TEMPLATE = app


SOURCES += main.cpp\
        game.cpp \
    http.cpp \
    user.cpp \
    card.cpp \
    style.cpp \
    chat.cpp \
    table.cpp \
    init_game.cpp \
    bidding.cpp \
    dealing_stock.cpp

HEADERS  += game.h \
    http.h \
    user.h \
    card.h \
    style.h \
    chat.h \
    table.h \
    init_game.h \
    bidding.h \
    dealing_stock.h
