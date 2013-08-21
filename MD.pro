TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -llapack -lblas -larmadillo -I/usr/lib/libarmadillo.so

INCLUDEPATH += /usr/include

SOURCES += main.cpp \
    src/solver/solver.cpp \
    src/Ensemble/ensemble.cpp \
    src/region/region.cpp \
    src/meshField/meshField.cpp

HEADERS += \
    src/solver/solver.h \
    src/Ensemble/ensemble.h \
    src/defines.h \
    src/region/region.h \
    src/meshField/meshField.h

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
