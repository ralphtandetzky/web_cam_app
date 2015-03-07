#-------------------------------------------------
#
# Project created by QtCreator 2015-03-07T10:30:02
#
#-------------------------------------------------

QT       += core gui multimedia multimediawidgets

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = web_cam_app
TEMPLATE = app


SOURCES += main.cpp\
    gui_web_cam_main_window.cpp

HEADERS  += \
    gui_web_cam_main_window.h

FORMS    += \
    gui_web_cam_main_window.ui
