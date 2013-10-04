TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -llapack -lblas -larmadillo -I/usr/lib/libarmadillo.so -lconfig++ -I/usr/include/python2.7 -lpython2.7

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

## MPI Settings
QMAKE_CXX = mpicxx
QMAKE_CXX_RELEASE = $$QMAKE_CXX
QMAKE_CXX_DEBUG = $$QMAKE_CXX
QMAKE_LINK = $$QMAKE_CXX

QMAKE_LFLAGS = $$system(mpicxx --showme:link)
QMAKE_CXXFLAGS = $$system(mpicxx --showme:compile) -DMPICH_IGNORE_CXX_SEEK
QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CXXFLAGS

#C++11 features
COMMON_CXXFLAGS = -std=c++0x
QMAKE_CXXFLAGS += $$COMMON_CXXFLAGS

QMAKE_CXXFLAGS_DEBUG += $$COMMON_CXXFLAGS -g

QMAKE_CXXFLAGS_RELEASE += $$COMMON_CXXFLAGS -O3 -DARMA_NO_DEBUG



