#-------------------------------------------------
#
# Project created by QtCreator 2014-06-29T10:30:01
#
# Note any additional files added requires qt 
# designer to run 'qmake' before build
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = project
TEMPLATE = app

SOURCES += \
    editor.cpp \
    main.cpp \
    tweakable_value.cpp \
    tweaker.cpp \
    tweakable_box.cpp

HEADERS  += \
    editor.h \
    tweakable_value.h \
    tweaker.h \
    tweakable_box.h

FORMS    += \
    editor.ui \
    tweaker.ui
