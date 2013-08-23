#include "meshField.h"

#include <sstream>
#include "../Ensemble/ensemble.h"



meshField::meshField(const mat &topology, Ensemble *ensemble, std::string description):
    topology(topology),
    description(description)
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



void meshField::append(int i)
{

    //Check whether atom 'i' is outside any of the region's bounds
    for (int j = 0; j < MD_DIM; ++j) {
        if (ensemble->pos(i, j) < topology(0, j)){
            return;
        } else if (ensemble->pos(i, j) > topology(1, j)) {
            return;
        }
    }

    atoms.push_back(i);

}
