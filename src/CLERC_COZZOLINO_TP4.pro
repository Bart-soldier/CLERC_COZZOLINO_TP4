#-------------------------------------------------
#
# Project created by QtCreator 2019-01-12T12:27:11
#
#-------------------------------------------------

QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

unix {
    LIBS += -lGLU -lm
}
win32 {
    LIBS += -lGLU32 -lOpengl32 -lm
}

TARGET = CLERC_COZZOLINO_TP4
TEMPLATE = app


SOURCES += main.cpp\
        link.cpp \
        princ.cpp \
        glarea.cpp

HEADERS  += princ.h \
        glarea.h \
        link.h

FORMS    += princ.ui

RESOURCES += \
    CLERC_COZZOLINO_TP4.qrc
