#ifndef MD_MESHFIELD_H
#define MD_MESHFIELD_H

#include "../defines.h"

#include <armadillo>
using namespace arma;

#include <string>
#include <vector>

class Ensemble;

class meshField
{

private:

    std::string description;

//    std::string type;
//    std::string unit;

    double volume;

    Ensemble *ensemble;

    std::vector<int> atoms;

    mat::fixed<2, MD_DIM> topology;

public:

    meshField(const mat & topology, Ensemble *ensemble,
              std::string description = "");

    virtual void update() = 0;

    virtual double getMeasurement(){
        return 0.0;
    }

    void append(int i);

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
