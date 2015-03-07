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

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../cpp_utils/release/ -lcpp_utils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../cpp_utils/debug/ -lcpp_utils
else:unix: LIBS += -L$$OUT_PWD/../cpp_utils/ -lcpp_utils

INCLUDEPATH += $$PWD/../cpp_utils
DEPENDPATH += $$PWD/../cpp_utils

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../cpp_utils/release/libcpp_utils.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../cpp_utils/debug/libcpp_utils.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../cpp_utils/release/cpp_utils.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../cpp_utils/debug/cpp_utils.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../cpp_utils/libcpp_utils.a

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../qt_utils/release/ -lqt_utils
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../qt_utils/debug/ -lqt_utils
else:unix: LIBS += -L$$OUT_PWD/../qt_utils/ -lqt_utils

INCLUDEPATH += $$PWD/../qt_utils
DEPENDPATH += $$PWD/../qt_utils

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qt_utils/release/libqt_utils.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qt_utils/debug/libqt_utils.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qt_utils/release/qt_utils.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../qt_utils/debug/qt_utils.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../qt_utils/libqt_utils.a
