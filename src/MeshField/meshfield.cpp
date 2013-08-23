#include "meshfield.h"

#include <sstream>
#include "../Ensemble/ensemble.h"
#include "../Event/event.h"



MeshField::MeshField(const mat &topology, Ensemble *ensemble, std::string description):
    topology(topology),
    description(description),
    nEvents(0)
{
    this->ensemble = ensemble;
}



//std::string meshField::dumpString()
//{
//    std::stringstream s;

//    s << "<"

//      << type << ", "
//      << description << ", "
//      << "value: " << getMeasurement() << " "
//      << unit

//      << " >";

//    return s.str();
//}



bool MeshField::isWithinThis(int i) {

    for (int j = 0; j < MD_DIM; ++j) {
        if (ensemble->pos(i, j) < topology(0, j)){
            return false;
        } else if (ensemble->pos(i, j) > topology(1, j)) {
            return false;
        }
    }

    return true;

}

void MeshField::addEvent(Event *event)
{
    event->setMeshField(this);
    event->setEnsemble(ensemble);
    events.push_back(event);
}
