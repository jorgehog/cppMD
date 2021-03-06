TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
#CONFIG += GUI

LIBS += -larmadillo -lconfig++ -I/usr/include/python2.7 -lpython2.7

INCLUDEPATH += /usr/include /usr/include/python2.7 /home/jorgehog/code/DCViz/include

SOURCES += main.cpp \
    src/MeshField/meshfield.cpp \
    src/Event/event.cpp \
    src/MeshField/MainMesh/mainmesh.cpp \
    src/MD/mdsolver.cpp \
    src/MD/forces/forces.cpp \
    src/MeshField/VerletCell/verletcell.cpp

HEADERS += \
    src/Ensemble/ensemble.h \
    src/defines.h \
    src/MeshField/meshfield.h \
    src/Event/event.h \
    src/MeshField/MainMesh/mainmesh.h \
    src/MD/mdsolver.h \
    src/Event/predefinedEvents/predefinedevents.h \
    src/gears.h \
    src/MD/forces/forces.h \
    cppMD.h

OTHER_FILES += configMD.cfg

QMAKE_CXX = g++
## MPI Settings
mpi {
    DEFINES += USE_MPI
    QMAKE_CXX = mpicxx
    QMAKE_CXX_RELEASE = $$QMAKE_CXX
    QMAKE_CXX_DEBUG = $$QMAKE_CXX
    QMAKE_LINK = $$QMAKE_CXX

    QMAKE_LFLAGS = $$system(mpicxx --showme:link)
    QMAKE_CXXFLAGS = $$system(mpicxx --showme:compile) -DMPICH_IGNORE_CXX_SEEK
}

QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CXXFLAGS

COMMON_CXXFLAGS = -std=c++0x


GUI {
    CONFIG += qt
    COMMON_CXXFLAGS += -DNO_DCVIZ
    QT       += core gui opengl
    greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

    HEADERS += GUI/cppMDGUI/viz/qtsprite.h \
        GUI/cppMDGUI/viz/qtplatform.h \
        GUI/cppMDGUI/viz/graphicsscene.h \
        GUI/cppMDGUI/viz/platform.h \
        GUI/cppMDGUI/viz/sprite.h \
        GUI/cppMDGUI/mainwindow.h \
        GUI/cppMDGUI/ui_mainwindow.h

    SOURCES += GUI/cppMDGUI/viz/qtsprite.cpp \
        GUI/cppMDGUI/viz/qtplatform.cpp \
        GUI/cppMDGUI/viz/graphicsscene.cpp \
        GUI/cppMDGUI/viz/platform.cpp \
        GUI/cppMDGUI/viz/sprite.cpp \
        GUI/cppMDGUI/mainwindow.cpp

    RESOURCES += \
        GUI/cppMDGUI/resources.qrc
}

QMAKE_CXXFLAGS += $$COMMON_CXXFLAGS

QMAKE_CXXFLAGS_DEBUG += $$COMMON_CXXFLAGS -g

QMAKE_CXXFLAGS_RELEASE += $$COMMON_CXXFLAGS -O3 -DARMA_NO_DEBUG -DNDEBUG

ccache {
    QMAKE_CXX = ccache $$QMAKE_CXX
}
