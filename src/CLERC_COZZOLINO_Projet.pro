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

TARGET = CLERC_COZZOLINO_Projet
TEMPLATE = app


SOURCES += main.cpp\
        cylinder.cpp \
        gear.cpp \
        kite.cpp \
        princ.cpp \
        glarea.cpp

HEADERS  += princ.h \
        cylinder.h \
        gear.h \
        glarea.h \
        kite.h

FORMS    += princ.ui

RESOURCES += \
    CLERC_COZZOLINO_Projet.qrc
