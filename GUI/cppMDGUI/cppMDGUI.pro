#-------------------------------------------------
#
# Project created by QtCreator 2013-10-04T17:28:10
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cppMDGUI
TEMPLATE = app

SOURCES += guiMain.cpp \
        mainwindow.cpp \
    ../../src/MeshField/meshfield.cpp \
    ../../src/Event/event.cpp \
    ../../src/MeshField/MainMesh/mainmesh.cpp \
    ../../src/MD/mdsolver.cpp \
    ../../src/MD/forces/forces.cpp \
    ../../src/MeshField/VerletCell/verletcell.cpp \
    viz/qtsprite.cpp \
    viz/qtplatform.cpp \
    viz/graphicsscene.cpp \
    viz/platform.cpp \
    viz/sprite.cpp

HEADERS += mainwindow.h \
    ../../src/Ensemble/ensemble.h \
    ../../src/defines.h \
    ../../src/MeshField/meshfield.h \
    ../../src/Event/event.h \
    ../../src/MeshField/MainMesh/mainmesh.h \
    ../../src/MD/mdsolver.h \
    ../../src/Event/predefinedEvents/predefinedevents.h \
    ../../src/gears.h \
    ../../src/MD/forces/forces.h \
    ../../cppMD.h \
    viz/qtsprite.h \
    viz/qtplatform.h \
    viz/graphicsscene.h \
    viz/platform.h \
    viz/sprite.h

FORMS    += mainwindow.ui

DEFINES += NO_DCVIZ

COMMON_CXXFLAGS = -std=c++0x
QMAKE_CXXFLAGS += $$COMMON_CXXFLAGS
QMAKE_CXXFLAGS -= O2

QMAKE_CXXFLAGS_DEBUG += -g
QMAKE_CXXFLAGS_RELEASE += -O3 -DARMA_NO_DEBUG


RESOURCES += \
    resources.qrc
