#ifndef MD_REGION_H
#define MD_REGION_H

#include "../defines.h"
#include <vector>
#include <string>
#include <armadillo>

using namespace arma;

class Ensemble;

class Region
{
public:

    std::string description;

    Region(const mat & topology, Ensemble* ensemble);

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

    bool equals(const Region & region) const;

private:

    Ensemble *ensemble;

    double volume;

    std::vector<int> atoms;

    mat::fixed<2, MD_DIM> topology;

};

#endif // MD_REGION_H
