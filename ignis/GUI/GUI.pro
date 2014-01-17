#-------------------------------------------------
#
# Project created by QtCreator 2013-10-04T17:28:10
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cppMDGUI
TEMPLATE = app

LIBS += ignis -L../
INCLUDEPATH += ..

SOURCES += guiMain.cpp \
        mainwindow.cpp \
    src/qtsprite.cpp \
    src/qtplatform.cpp \
    src/graphicsscene.cpp \
    src/platform.cpp \
    src/sprite.cpp

HEADERS += mainwindow.h \
    src/qtsprite.h \
    src/qtplatform.h \
    src/graphicsscene.h \
    src/platform.h \
    src/sprite.h

FORMS    += mainwindow.ui

DEFINES += NO_DCVIZ

COMMON_CXXFLAGS = -std=c++0x
QMAKE_CXXFLAGS += $$COMMON_CXXFLAGS
QMAKE_CXXFLAGS -= O2

QMAKE_CXXFLAGS_DEBUG += -g
QMAKE_CXXFLAGS_RELEASE += -O3 -DARMA_NO_DEBUG


RESOURCES += \
    resources.qrc
