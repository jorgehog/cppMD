include(../../ignisdefines.pri)

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += GUI

TARGET = MD

LIBS += -larmadillo -lconfig++ -L$$TOP_OUT_PWD/ignis/lib -lignis

INCLUDEPATH += $$TOP_PWD/ignis/include $(HOME)/Dropbox/libs

SOURCES += main.cpp \
           mdsolver.cpp \
           forces/forces.cpp \
    mdevent.cpp

HEADERS += mdsolver.h \
           forces/forces.h \
           gears.h \
           mdevents.h \
    mdevent.h

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

QMAKE_CXXFLAGS += $$COMMON_CXXFLAGS

QMAKE_CXXFLAGS_DEBUG += $$COMMON_CXXFLAGS -g

QMAKE_CXXFLAGS_RELEASE += $$COMMON_CXXFLAGS -O3 -DARMA_NO_DEBUG -DNDEBUG

ccache {
    QMAKE_CXX = ccache $$QMAKE_CXX
}
