#ifndef MD_MESHFIELD_H
#define MD_MESHFIELD_H

#include "../defines.h"

#include <armadillo>
using namespace arma;

#include <string>
#include <vector>

class Ensemble;
class Event;

class MeshField
{

private:

    std::string description;

    int nEvents;
    double volume;

    Ensemble *ensemble;

    mat::fixed<2, MD_DIM> topology;
    std::vector<int> atoms;
    std::vector<Event*> events;


public:

    MeshField(const mat & topology, Ensemble *ensemble,
              std::string description = "");

    virtual bool isWithinThis(int i);

    void addEvent(Event* event);

    void append(int i) {
        if (isWithinThis(i)) atoms.push_back(i);
    }

    void reset(){
        atoms.clear();
    }

    rowvec getShape() const {
        return topology.row(1) - topology.row(0);
    }

    double getVolume() const {
        return volume;
    }

//    std::string dumpString();


};

#endif // MD_MESHFIELD_H
