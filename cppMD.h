#ifndef CPPMD_H
#define CPPMD_H

#include "src/defines.h"

#include "src/Ensemble/ensemble.h"
#include "src/MeshField/meshfield.h"
#include "src/MeshField/MainMesh/mainmesh.h"

#include "src/Event/event.h"
#include "src/Event/predefinedEvents/predefinedevents.h"
#include "src/MD/mdsolver.h"

#include "src/MD/forces/forces.h"

#ifdef NO_DCVIZ
#include <QApplication>
#include "GUI/cppMDGUI/viz/qtplatform.h"
#include "GUI/cppMDGUI/mainwindow.h"
#endif

#endif // CPPMD_H
