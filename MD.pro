TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -llapack -lblas -larmadillo -I/usr/lib/libarmadillo.so -lconfig++

INCLUDEPATH += /usr/include

SOURCES += main.cpp \
    src/MeshField/meshfield.cpp \
    src/Event/event.cpp \
    src/MeshField/MainMesh/mainmesh.cpp \
    src/Event/SolverEvent/solverevent.cpp \
    src/MD/mdsolver.cpp \
    src/MD/mdEvents/calculateForces/calculateforces.cpp \
    src/Event/predefinedEvents/predefinedevents.cpp

HEADERS += \
    src/Ensemble/ensemble.h \
    src/defines.h \
    src/MeshField/meshfield.h \
    src/Event/event.h \
    src/MeshField/MainMesh/mainmesh.h \
    src/Event/SolverEvent/solverevent.h \
    src/MD/mdsolver.h \
    src/MD/mdEvents/calculateForces/calculateforces.h \
    src/Event/predefinedEvents/predefinedevents.h

OTHER_FILES += configMD.cfg

## MPI Settings
#QMAKE_CXX = mpicxx
#QMAKE_CXX_RELEASE = $$QMAKE_CXX
#QMAKE_CXX_DEBUG = $$QMAKE_CXX
#QMAKE_LINK = $$QMAKE_CXX
#QMAKE_CC = mpicc

#QMAKE_CFLAGS = $$system(mpicc --showme:compile)
#QMAKE_LFLAGS = $$system(mpicxx --showme:link)
#QMAKE_CXXFLAGS = $$system(mpicxx --showme:compile) -DMPICH_IGNORE_CXX_SEEK
#QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CXXFLAGS

#C++11 features
COMMON_CXXFLAGS = -std=c++0x
QMAKE_CXXFLAGS += $$COMMON_CXXFLAGS
QMAKE_CXXFLAGS_RELEASE += $$COMMON_CXXFLAGS
QMAKE_CXXFLAGS_DEBUG += $$COMMON_CXXFLAGS

    DEFINES += ARMA_NO_DEBUG
    QMAKE_LFLAGS -= -O1
    QMAKE_LFLAGS += -O3
    QMAKE_LFLAGS_RELEASE -= -O1
    QMAKE_LFLAGS_RELEASE += -O3
    QMAKE_CXXFLAGS -= -O2
    QMAKE_CXXFLAGS += -O3
    QMAKE_CXXFLAGS_RELEASE -= -O2
    QMAKE_CXXFLAGS_RELEASE += -O3

