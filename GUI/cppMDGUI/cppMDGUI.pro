#-------------------------------------------------
#
# Project created by QtCreator 2013-10-04T17:28:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cppMDGUI
TEMPLATE = app

LIBS += -llapack -lblas -larmadillo -I/usr/lib/libarmadillo.so -lpython2.7

INCLUDEPATH += /usr/include/python2.7

SOURCES += guiMain.cpp \
        mainwindow.cpp \
    ../../src/MeshField/meshfield.cpp \
    ../../src/Event/event.cpp \
    ../../src/MeshField/MainMesh/mainmesh.cpp \
    ../../src/MD/mdsolver.cpp \
    ../../src/MD/forces/forces.cpp \
    ../../src/MeshField/VerletCell/verletcell.cpp

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
    ../../cppMD.h

FORMS    += mainwindow.ui

COMMON_CXXFLAGS = -std=c++0x
QMAKE_CXXFLAGS += $$COMMON_CXXFLAGS
