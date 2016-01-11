#-------------------------------------------------
#
# Project created by QtCreator 2014-11-14T11:50:15
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#LIBS += C:\OpenCV243\opencv\build\x86\vc10\bin

TARGET = PlayMidiNote
TEMPLATE = app


SOURCES += main.cpp\
    soundobject.cpp \
    node.cpp

HEADERS  += \
    soundobject.h \
    node.h \
    ../RtMidi/rtmidi-master/RtMidi.h

FORMS
include(../drumstick/drumstick.pro)
