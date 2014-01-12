#ifndef IGNIS_H
#define IGNIS_H

namespace ignis {

#include "src/defines.h"

#include "src/Ensemble/ensemble.h"
#include "src/MeshField/meshfield.h"
#include "src/MeshField/MainMesh/mainmesh.h"

#include "src/Event/event.h"
#include "src/Event/predefinedEvents/predefinedevents.h"

#ifdef NO_DCVIZ
#include <QApplication>
#include "GUI/cppMDGUI/viz/qtplatform.h"
#include "GUI/cppMDGUI/mainwindow.h"
#endif

} //end namespace ignis


#endif // IGNIS_H
